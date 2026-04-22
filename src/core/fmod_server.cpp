#include "audio/fmod_sound.h"
#include "dsp/fmod_audio_effect.h"
#include "fmod_server.h"
#include "mixer/fmod_audio_bus.h"
#include "mixer/fmod_audio_bus_layout.h"
#include "playback/fmod_channel.h"
#include "playback/fmod_channel_group.h"

#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/classes/camera2d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_script_picker.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/performance.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

// 用于获取编辑器视口的辅助函数
#ifdef TOOLS_ENABLED
static godot::Viewport* _get_editor_viewport_2d() {
	godot::EditorInterface* editor = godot::EditorInterface::get_singleton();
	if (!editor) return nullptr;
	// 获取当前编辑场景的根视口
	return editor->get_editor_viewport_2d();
}

static godot::SubViewport* _get_editor_viewport_3d(int p_index = 0) {
	godot::EditorInterface* editor = godot::EditorInterface::get_singleton();
	if (!editor) return nullptr;
	return editor->get_editor_viewport_3d(p_index);
}
#endif

namespace godot {
	FmodServer* FmodServer::singleton = nullptr;
	Ref<FmodSystem> FmodServer::main_system;
	Ref<FmodAudioBusLayout> FmodServer::audio_bus_layout = Ref<FmodAudioBusLayout>();
	Vector2 FmodServer::camera_2d_pos;
	float FmodServer::camera_2d_rot = 0.0f;

	void FmodServer::_bind_methods() {
		ClassDB::bind_method(D_METHOD("_connect_update"), &FmodServer::_connect_update);
		ClassDB::bind_static_method("FmodServer", D_METHOD("get_singleton"), &FmodServer::get_singleton);
		ClassDB::bind_static_method("FmodServer", D_METHOD("get_main_system"), &FmodServer::get_main_system);
		ClassDB::bind_static_method("FmodServer", D_METHOD("get_master_channel_group"), &FmodServer::get_master_channel_group);
		ClassDB::bind_static_method("FmodServer", D_METHOD("get_audio_bus_layout"), &FmodServer::get_audio_bus_layout);
		ClassDB::bind_static_method("FmodServer", D_METHOD("generate_bus_layout"), &FmodServer::generate_bus_layout);
		ClassDB::bind_static_method("FmodServer", D_METHOD("reset_main_system", "system"), &FmodServer::reset_main_system);
	}

	FmodServer::FmodServer() {
		ERR_FAIL_COND(singleton != nullptr);
		singleton = this;

		ProjectSettings* settings = ProjectSettings::get_singleton();

		int max_channels = 32;
		if (settings && settings->has_setting("audio/fmod/max_channels")) {
			max_channels = settings->get_setting("audio/fmod/max_channels");
		}

		bool enable_profile = true;
		FmodSystem::FmodInitFlags mode = FmodSystem::FMOD_INIT_FLAG_NORMAL;
		if (Engine::get_singleton()->is_editor_hint()) {
			if (settings && settings->has_setting("audio/fmod/enable_profile")) {
				enable_profile = settings->get_setting("audio/fmod/enable_profile");
			}
			if (enable_profile) {
				mode = FmodSystem::FMOD_INIT_FLAG_PROFILE_ENABLE;
			}
		}

		main_system = FmodSystem::create_system();
		if (main_system.is_null() || main_system->system_is_null()) {
			UtilityFunctions::print_rich("[b][color=WHITE][bgcolor=RED]Failed to init main system![/bgcolor][/color][/b]");
			return;
		}
		main_system->init(max_channels, mode);

		if (Engine::get_singleton()->is_editor_hint() && enable_profile && settings && settings->has_setting("audio/fmod/network_proxy")) {
			String proxy = settings->get_setting("audio/fmod/network_proxy");
			main_system->set_network_proxy(proxy);
		}

		bool show_startup_banner = true;
		if (settings && settings->has_setting("audio/fmod/show_startup_banner")) {
			show_startup_banner = settings->get_setting("audio/fmod/show_startup_banner");
		}
		if (show_startup_banner) {
			UtilityFunctions::print("    _____                    _ ____  _                       ");
			UtilityFunctions::print("   |  ___| __ ___   ___   __| |  _ \\| | __ _ _   _  ___ _ __ ");
			UtilityFunctions::print("   | |_ | '_ ` _ \\ / _ \\ / _` | |_) | |/ _` | | | |/ _ \\ '__|");
			UtilityFunctions::print("   |  _|| | | | | | (_) | (_| |  __/| | (_| | |_| |  __/ |   ");
			UtilityFunctions::print("   |_|  |_| |_| |_|\\___/ \\__,_|_|   |_|\\__,_|\\__, |\\___|_|   ");
			UtilityFunctions::print("                                             |___/           ");
			Dictionary version = main_system->get_version();
			UtilityFunctions::print_rich(
				"[b][color=BLACK][bgcolor=WHITE]Fmod Completed.\tFmod Version: ", 
				version.get("version", "Unknow"),
				"\tFmod Build Number: ",
				version.get("build_number", "Unknow"),
				"[/bgcolor][/color][/b]"
			);
		}

		// 初始化总线布局
		audio_bus_layout.instantiate();
	}

	FmodServer::~FmodServer() {
		ERR_FAIL_COND(singleton != this);
		singleton = nullptr;

		// 释放 FMODSystem
		main_system->release();

		// 注销自定义监视器
		Performance* perf = Performance::get_singleton();
		if (perf) {
			perf->remove_custom_monitor("FmodCPUUsage/DSP");
			perf->remove_custom_monitor("FmodCPUUsage/Stream");
			perf->remove_custom_monitor("FmodCPUUsage/Geometry");
			perf->remove_custom_monitor("FmodCPUUsage/Update");
			perf->remove_custom_monitor("FmodCPUUsage/Convolution1");
			perf->remove_custom_monitor("FmodCPUUsage/Convolution2");
			perf->remove_custom_monitor("FmodFileUsage/SampleBytesRead");
			perf->remove_custom_monitor("FmodFileUsage/StreamBytesRead");
			perf->remove_custom_monitor("FmodFileUsage/OtherBytesRead");
		}
	}

	void FmodServer::_notification(int p_what) {
		switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			call_deferred("_connect_update");
		} break;
		}
	}

	FmodServer* FmodServer::get_singleton() {
		return singleton;
	}

	void FmodServer::_connect_update() {
		SceneTree* tree = Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
		if (tree) {
			tree->connect("process_frame", callable_mp(get_singleton(), &FmodServer::_update_fmod));

			Performance* perf = Performance::get_singleton();
			if (perf) {
				perf->add_custom_monitor("FmodCPUUsage/DSP", callable_mp(this, &FmodServer::_get_dsp));
				perf->add_custom_monitor("FmodCPUUsage/Stream", callable_mp(this, &FmodServer::_get_stream));
				perf->add_custom_monitor("FmodCPUUsage/Geometry", callable_mp(this, &FmodServer::_get_geometry));
				perf->add_custom_monitor("FmodCPUUsage/Update", callable_mp(this, &FmodServer::_get_update));
				perf->add_custom_monitor("FmodCPUUsage/Convolution1", callable_mp(this, &FmodServer::_get_convolution1));
				perf->add_custom_monitor("FmodCPUUsage/Convolution2", callable_mp(this, &FmodServer::_get_convolution2));
				perf->add_custom_monitor("FmodFileUsage/SampleBytesRead", callable_mp(this, &FmodServer::_get_sample_bytes_read));
				perf->add_custom_monitor("FmodFileUsage/StreamBytesRead", callable_mp(this, &FmodServer::_get_stream_bytes_read));
				perf->add_custom_monitor("FmodFileUsage/OtherBytesRead", callable_mp(this, &FmodServer::_get_other_bytes_read));
			}
			else {
				WARN_PRINT("Failed to get Performance!");
			}

			AudioServer* audio_server = AudioServer::get_singleton();
			if (audio_server) {
				audio_server->connect("bus_layout_changed", callable_mp(this, &FmodServer::_build_bus_layout), CONNECT_DEFERRED);
				_build_bus_layout();
			}
			else {
				WARN_PRINT("Failed to get AudioServer!");
			}
		}
		else {
			ERR_PRINT("Failed to get SceneTree!");
		}
	}

	void FmodServer::_update_fmod() {
		if (!singleton || main_system.is_null()) return;
		
		// 更新监听者位置
		SceneTree* tree = Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
		if (tree) {
			Viewport* viewport = tree->get_root();
			
			if (Engine::get_singleton()->is_editor_hint()) {
				EditorInterface* editor = EditorInterface::get_singleton();
				if (editor) {
					SubViewport* editor_viewport_3d = editor->get_editor_viewport_3d(0);
					if (editor_viewport_3d) {
						Camera3D* camera_3d = editor_viewport_3d->get_camera_3d();
						if (camera_3d) {
							Vector3 position = camera_3d->get_global_position();
							Vector3 forward = -camera_3d->get_global_transform().get_basis().get_column(2).normalized();
							Vector3 up = camera_3d->get_global_transform().get_basis().get_column(1).normalized();
							main_system->set_3d_listener_attributes(
								0,
								position,
								Vector3(0, 0, 0),
								forward,
								up
							);
							main_system->update();
						}
					}
					
					Viewport* editor_viewport_2d = editor->get_editor_viewport_2d();
					if (editor_viewport_2d) {
						Transform2D transform = editor_viewport_2d->get_global_canvas_transform();
						camera_2d_pos = transform.get_origin();
						camera_2d_rot = transform.get_rotation();
					}

					return;
				}
			}
			
			// 运行时或编辑器中未找到编辑器相机，使用游戏视口
			if (viewport) {
				Camera3D* camera_3d = viewport->get_camera_3d();
				if (camera_3d) {
					Vector3 position = camera_3d->get_global_position();
					Vector3 forward = -camera_3d->get_global_transform().get_basis().get_column(2).normalized();
					Vector3 up = camera_3d->get_global_transform().get_basis().get_column(1).normalized();
					main_system->set_3d_listener_attributes(
						0,
						position,
						Vector3(0, 0, 0),
						forward,
						up
					);
					main_system->update();
				}
				else {
					Camera2D* camera_2d = viewport->get_camera_2d();
					if (camera_2d) {
						camera_2d_pos = camera_2d->get_global_position();
						camera_2d_rot = camera_2d->get_global_rotation();
					}
				}
			}
		}
	}

	Ref<FmodSystem> FmodServer::get_main_system() {
		return main_system;
	}

	Ref<FmodChannelGroup> FmodServer::get_master_channel_group() {
		if (main_system.is_null()) {
			return Ref<FmodChannelGroup>();
		}
		return main_system->get_master_channel_group();
	}

	Ref<FmodAudioBusLayout> FmodServer::get_audio_bus_layout() {
		return audio_bus_layout;
	}

	void FmodServer::generate_bus_layout() {
		audio_bus_layout->sync_from_audio_server();
	}

	void FmodServer::reset_main_system(Ref<FmodSystem> p_system) {
		ERR_FAIL_COND_MSG(p_system.is_null() || p_system->system_is_null(), "New system is null");
		if (main_system.is_valid()) {
			main_system->release();
		}
		main_system = p_system;
	}

	void FmodServer::_build_bus_layout() {
		if (audio_bus_layout.is_valid()) {
			audio_bus_layout->sync_from_audio_server();
		}
	}

	double FmodServer::_get_dsp() const {
		if (!singleton || main_system.is_null()) return 0.0;
		return main_system->get_cpu_usage().get("dsp", 0.0);
	}

	double FmodServer::_get_stream() const {
		if (!singleton || main_system.is_null()) return 0.0;
		return main_system->get_cpu_usage().get("stream", 0.0);
	}

	double FmodServer::_get_geometry() const {
		if (!singleton || main_system.is_null()) return 0.0;
		return main_system->get_cpu_usage().get("geometry", 0.0);
	}

	double FmodServer::_get_update() const {
		if (!singleton || main_system.is_null()) return 0.0;
		return main_system->get_cpu_usage().get("update", 0.0);
	}

	double FmodServer::_get_convolution1() const {
		if (!singleton || main_system.is_null()) return 0.0;
		return main_system->get_cpu_usage().get("convolution1", 0.0);
	}

	double FmodServer::_get_convolution2() const {
		if (!singleton || main_system.is_null()) return 0.0;
		return main_system->get_cpu_usage().get("convolution2", 0.0);
	}

	int64_t FmodServer::_get_sample_bytes_read() const {
		if (!singleton || main_system.is_null()) return 0;
		return main_system->get_file_usage().get("sample_bytes_read", 0);
	}

	int64_t FmodServer::_get_stream_bytes_read() const {
		if (!singleton || main_system.is_null()) return 0;
		return main_system->get_file_usage().get("stream_bytes_read", 0);
	}

	int64_t FmodServer::_get_other_bytes_read() const {
		if (!singleton || main_system.is_null()) return 0;
		return main_system->get_file_usage().get("other_bytes_read", 0);
	}
}
