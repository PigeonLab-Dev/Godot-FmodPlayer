#include "playback/fmod_channel.h"
#include "playback/fmod_channel_group.h"
#include "fmod_server.h"
#include "audio/fmod_sound.h"
#include "mixer/fmod_audio_bus.h"
#include "mixer/fmod_audio_bus_layout.h"
#include "dsp/fmod_audio_effect.h"

#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/window.hpp>

namespace godot {
	FmodServer* FmodServer::singleton = nullptr;
	FmodSystem* FmodServer::main_system = nullptr;
	Ref<FmodAudioBusLayout> FmodServer::audio_bus_layout = Ref<FmodAudioBusLayout>();

	void FmodServer::_bind_methods() {
		ClassDB::bind_method(D_METHOD("_connect_update"), &FmodServer::_connect_update);
		ClassDB::bind_static_method("FmodServer", D_METHOD("get_singleton"), &FmodServer::get_singleton);
		ClassDB::bind_static_method("FmodServer", D_METHOD("get_main_system"), &FmodServer::get_main_system);
		ClassDB::bind_static_method("FmodServer", D_METHOD("get_master_channel_group"), &FmodServer::get_master_channel_group);
		ClassDB::bind_static_method("FmodServer", D_METHOD("get_audio_bus_layout"), &FmodServer::get_audio_bus_layout);
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

		main_system = FmodSystem::create_system(max_channels, mode);
		if (!main_system || main_system->system_is_null()) {
			UtilityFunctions::print_rich("[b][color=WHITE][bgcolor=RED]Failed to init main system![/bgcolor][/color][/b]");
			return;
		}

		if (Engine::get_singleton()->is_editor_hint() && enable_profile && settings && settings->has_setting("audio/fmod/network_proxy")) {
			String proxy = settings->get_setting("audio/fmod/network_proxy");
			main_system->set_network_proxy(proxy);
		}

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

		// 初始化总线布局
		audio_bus_layout.instantiate();
	}

	FmodServer::~FmodServer() {
		ERR_FAIL_COND(singleton != this);
		singleton = nullptr;

		// 释放 FmodSystem
		if (main_system) {
			memdelete(main_system);
			main_system = nullptr;
		}

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
				UtilityFunctions::push_warning("Failed to get Performance!");
			}

			AudioServer* audio_server = AudioServer::get_singleton();
			if (audio_server) {
				audio_server->connect("bus_layout_changed", callable_mp(this, &FmodServer::_build_bus_layout), CONNECT_DEFERRED);
				_build_bus_layout();
			}
			else {
				UtilityFunctions::push_warning("Failed to get AudioServer!");
			}
		}
		else {
			UtilityFunctions::push_error("Failed to get SceneTree!");
		}
	}

	void FmodServer::_update_fmod() {
		if (!singleton || !main_system) return;
		
		// 更新 3D 监听者位置
		SceneTree* tree = Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
		if (tree) {
			Viewport* viewport = tree->get_root();
			if (viewport) {
				Camera3D* camera = viewport->get_camera_3d();
				if (camera) {
					// 获取相机的位置和方向
					Vector3 position = camera->get_global_position();
					Vector3 forward = -camera->get_global_transform().get_basis().get_column(2).normalized();
					Vector3 up = camera->get_global_transform().get_basis().get_column(1).normalized();
					
					// 设置 FMOD 监听者属性
					main_system->set_3d_listener_attributes(
						0,                          // listener index
						position,                   // position
						Vector3(0, 0, 0),          	// velocity (可以后续添加速度追踪)
						forward,                    // forward
						up                          // up
					);
				}
			}
		}
		
		main_system->update();
	}

	FmodSystem* FmodServer::get_main_system() {
		return main_system;
	}

	Ref<FmodChannelGroup> FmodServer::get_master_channel_group() {
		if (!main_system) {
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

	void FmodServer::_build_bus_layout() {
		if (audio_bus_layout.is_valid()) {
			audio_bus_layout->sync_from_audio_server();
		}
	}

	double FmodServer::_get_dsp() const {
		if (!singleton || !main_system) return 0.0;
		return main_system->get_cpu_usage().get("dsp", 0.0);
	}

	double FmodServer::_get_stream() const {
		if (!singleton || !main_system) return 0.0;
		return main_system->get_cpu_usage().get("stream", 0.0);
	}

	double FmodServer::_get_geometry() const {
		if (!singleton || !main_system) return 0.0;
		return main_system->get_cpu_usage().get("geometry", 0.0);
	}

	double FmodServer::_get_update() const {
		if (!singleton || !main_system) return 0.0;
		return main_system->get_cpu_usage().get("update", 0.0);
	}

	double FmodServer::_get_convolution1() const {
		if (!singleton || !main_system) return 0.0;
		return main_system->get_cpu_usage().get("convolution1", 0.0);
	}

	double FmodServer::_get_convolution2() const {
		if (!singleton || !main_system) return 0.0;
		return main_system->get_cpu_usage().get("convolution2", 0.0);
	}

	int64_t FmodServer::_get_sample_bytes_read() const {
		if (!singleton || !main_system) return 0;
		return main_system->get_file_usage().get("sample_bytes_read", 0);
	}

	int64_t FmodServer::_get_stream_bytes_read() const {
		if (!singleton || !main_system) return 0;
		return main_system->get_file_usage().get("stream_bytes_read", 0);
	}

	int64_t FmodServer::_get_other_bytes_read() const {
		if (!singleton || !main_system) return 0;
		return main_system->get_file_usage().get("other_bytes_read", 0);
	}
}
