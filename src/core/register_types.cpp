#include "register_types.h"

#include "audio/fmod_audio_stream.h"
#include "audio/fmod_audio_stream_flac.h"
#include "audio/fmod_sound.h"
#include "audio/fmod_sound_lock.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "dsp/fmod_audio_effect.h"
#include "dsp/fmod_audio_effect_amplify.h"
#include "dsp/fmod_audio_effect_capture.h"
#include "dsp/fmod_audio_effect_chorus.h"
#include "dsp/fmod_audio_effect_compressor.h"
#include "dsp/fmod_audio_effect_delay.h"
#include "dsp/fmod_audio_effect_distortion.h"
#include "dsp/fmod_audio_effect_eq.h"
#include "dsp/fmod_audio_effect_filter.h"
#include "dsp/fmod_audio_effect_hard_limiter.h"
#include "dsp/fmod_audio_effect_panner.h"
#include "dsp/fmod_audio_effect_phaser.h"
#include "dsp/fmod_audio_effect_pitch_shift.h"
#include "dsp/fmod_audio_effect_record.h"
#include "dsp/fmod_audio_effect_reverb.h"
#include "dsp/fmod_audio_effect_spectrum_analyzer.h"
#include "dsp/fmod_audio_effect_stereo_enhance.h"
#include "dsp/fmod_dsp.h"
#include "dsp/fmod_dsp_connection.h"
#include "editor/fmod_audio_importer.h"
#include "editor/fmod_audio_preview_inspector.h"
#include "editor/fmod_audio_preview_property.h"
#include "editor/fmod_audio_stream_player_3d_gizmo_plugin.h"
#include "geometry/fmod_geometry.h"
#include "mixer/fmod_audio_bus.h"
#include "mixer/fmod_audio_bus_layout.h"
#include "nodes/fmod_audio_sample_emitter.h"
#include "nodes/fmod_audio_stream_player.h"
#include "nodes/fmod_audio_stream_player_2d.h"
#include "nodes/fmod_audio_stream_player_3d.h"
#include "nodes/fmod_geometry_instance_3d.h"
#include "playback/fmod_channel.h"
#include "playback/fmod_channel_control.h"
#include "playback/fmod_channel_group.h"
#include "playback/fmod_sound_group.h"
#include "spatial/fmod_reverb_3d.h"

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

static FmodServer* fmod_server_instance = nullptr;

void initialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		// 注册自定义项目设置
		ProjectSettings* settings = ProjectSettings::get_singleton();
		
		// FMOD Profile 高级设置：仅在编辑器中初始化时启用 FMOD Profile (修改需要重启编辑器)
		if (!settings->has_setting("audio/fmod/enable_profile")) {
			settings->set_setting("audio/fmod/enable_profile", true);
		}
		settings->set_initial_value("audio/fmod/enable_profile", true);
		settings->set_as_basic("audio/fmod/enable_profile", true);
		settings->set_restart_if_changed("audio/fmod/enable_profile", true);
		Dictionary property_info_enable_profile;
		property_info_enable_profile["name"] = "audio/fmod/enable_profile";
		property_info_enable_profile["type"] = Variant::BOOL;
		settings->add_property_info(property_info_enable_profile);

		// FMOD Network Proxy 高级设置：仅在编辑器中初始化时启用 FMOD 网络代理 (修改需要重启编辑器)
		if (!settings->has_setting("audio/fmod/network_proxy")) {
			settings->set_setting("audio/fmod/network_proxy", "");
		}
		settings->set_initial_value("audio/fmod/network_proxy", "");
		settings->set_as_basic("audio/fmod/network_proxy", true);
		settings->set_restart_if_changed("audio/fmod/network_proxy", true);
		Dictionary property_info_network_proxy;
		property_info_network_proxy["name"] = "audio/fmod/network_proxy";
		property_info_network_proxy["type"] = Variant::STRING;
		settings->add_property_info(property_info_network_proxy);

		// FMOD System 最大通道数高级设置：初始化时指定主系统的最大通道数 (修改需要重启编辑器)
		if (!settings->has_setting("audio/fmod/max_channels")) {
			settings->set_setting("audio/fmod/max_channels", 32);
		}
		settings->set_initial_value("audio/fmod/max_channels", 32);
		settings->set_as_basic("audio/fmod/max_channels", true);
		settings->set_restart_if_changed("audio/fmod/max_channels", true);
		Dictionary property_info_max_channels;
		property_info_max_channels["name"] = "audio/fmod/max_channels";
		property_info_max_channels["type"] = Variant::INT;
		property_info_max_channels["hint"] = PROPERTY_HINT_RANGE;
		property_info_max_channels["hint_string"] = "1,4095,1";
		settings->add_property_info(property_info_max_channels);
		
		// 编辑器特有的类
		GDREGISTER_CLASS(AudioImporterFmod);
		GDREGISTER_CLASS(FmodAudioPreviewInspector);
		GDREGISTER_CLASS(FmodAudioPreviewProperty);
		GDREGISTER_CLASS(FmodAudioStreamPlayer3DGizmoPlugin);
		return;
	}
	
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// 音频核心类
	GDREGISTER_CLASS(FmodChannelControl);
	GDREGISTER_CLASS(FmodChannel);
	GDREGISTER_CLASS(FmodChannelGroup);
	GDREGISTER_CLASS(FmodSoundGroup);
	GDREGISTER_CLASS(FmodDSPConnection);
	GDREGISTER_CLASS(FmodDSP);
	GDREGISTER_CLASS(FmodSound);
	GDREGISTER_CLASS(FmodSoundLock);
	GDREGISTER_CLASS(FmodSystem);

	// 音频效果类
	GDREGISTER_ABSTRACT_CLASS(FmodAudioEffect);
	GDREGISTER_CLASS(FmodAudioEffectAmplify);
	GDREGISTER_CLASS(FmodAudioEffectFilter);
	GDREGISTER_CLASS(FmodAudioEffectCapture);
	GDREGISTER_CLASS(FmodAudioEffectChorus);
	GDREGISTER_CLASS(FmodAudioEffectCompressor);
	GDREGISTER_CLASS(FmodAudioEffectDelay);
	GDREGISTER_CLASS(FmodAudioEffectDistortion);
	GDREGISTER_CLASS(FmodAudioEffectEQ);
	GDREGISTER_CLASS(FmodAudioEffectEQ6);
	GDREGISTER_CLASS(FmodAudioEffectEQ10);
	GDREGISTER_CLASS(FmodAudioEffectEQ21);
	GDREGISTER_CLASS(FmodAudioEffectHardLimiter);
	GDREGISTER_CLASS(FmodAudioEffectPanner);
	GDREGISTER_CLASS(FmodAudioEffectPhaser);
	GDREGISTER_CLASS(FmodAudioEffectPitchShift);
	GDREGISTER_CLASS(FmodAudioEffectRecord);
	GDREGISTER_CLASS(FmodAudioEffectReverb);
	GDREGISTER_CLASS(FmodAudioEffectSpectrumAnalyzer);
	GDREGISTER_CLASS(FmodAudioEffectStereoEnhance);

	// 音频总线类
	GDREGISTER_CLASS(FmodAudioBus);
	GDREGISTER_CLASS(FmodAudioBusLayout);

	// 音频流类
	GDREGISTER_CLASS(FmodAudioStream);
	GDREGISTER_CLASS(FmodAudioStreamFLAC);

	// 空间
	GDREGISTER_CLASS(FmodReverb3D);

	// 几何体
	GDREGISTER_CLASS(FmodGeometry);

	// 音频播放器类
	GDREGISTER_CLASS(FmodAudioStreamPlayer);
	GDREGISTER_CLASS(FmodAudioStreamPlayer2D);
	GDREGISTER_CLASS(FmodAudioStreamPlayer3D);
	GDREGISTER_CLASS(FmodAudioSampleEmitter);
	GDREGISTER_CLASS(FmodGeometryInstance3D);

	// FmodServer 注册和添加单例
	GDREGISTER_CLASS(FmodServer);
	fmod_server_instance = memnew(FmodServer);
	Engine::get_singleton()->register_singleton("FmodServer", FmodServer::get_singleton());
}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if (fmod_server_instance) {
		memdelete(fmod_server_instance);
		fmod_server_instance = nullptr;
		Engine::get_singleton()->unregister_singleton("FmodServer");
	}
}

extern "C" {
	GDExtensionBool GDE_EXPORT fmod_player_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_example_module);
		init_obj.register_terminator(uninitialize_example_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);

		return init_obj.init();
	}
}
