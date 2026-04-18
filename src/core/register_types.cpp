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
#include "editor/modify_android_build_template_plugin.h"
#include "geometry/fmod_geometry.h"
#include "mixer/fmod_audio_bus.h"
#include "mixer/fmod_audio_bus_layout.h"
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

void initialize_fmod_player_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		ProjectSettings* settings = ProjectSettings::get_singleton();
		
#define ADD_SETTING_PROPERTY_INFO(m_name, m_value, m_initial_value, m_as_basic, m_restart_if_changed, m_type)	\
	if (!settings->has_setting(m_name)) {																		\
		settings->set_setting(m_name, m_value);																	\
	}																											\
	settings->set_initial_value(m_name, m_initial_value);														\
	settings->set_as_basic(m_name, m_as_basic);																	\
	settings->set_restart_if_changed(m_name, m_restart_if_changed);												\
	{																											\
		Dictionary _property_info;																				\
		_property_info["name"] = m_name;																		\
		_property_info["type"] = m_type;																		\
		settings->add_property_info(_property_info);															\
	}

		// FMOD Profile 高级设置 - 仅在编辑器中初始化时启用 FMOD Profile
		ADD_SETTING_PROPERTY_INFO("audio/fmod/enable_profile", true, true, true, true, Variant::BOOL)

		// FMOD Network Proxy 高级设置 - 仅在编辑器中初始化时启用 FMOD 网络代理
		ADD_SETTING_PROPERTY_INFO("audio/fmod/network_proxy", "", "", true, true, Variant::STRING)

		// FMOD 启动横幅显示设置 - 控制初始化时是否打印 FMODPlayer ASCII 艺术字横幅
		ADD_SETTING_PROPERTY_INFO("audio/fmod/show_startup_banner", true, true, true, false, Variant::BOOL)

		// FMOD System 最大通道数高级设置 - 初始化时指定主系统的最大通道数
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

#undef ADD_SETTING_PROPERTY_INFO

		GDREGISTER_CLASS(AudioImporterFmod);
		GDREGISTER_CLASS(FmodAudioPreviewInspector);
		GDREGISTER_CLASS(FmodAudioPreviewProperty);
		GDREGISTER_CLASS(FmodAudioStreamPlayer3DGizmoPlugin);
		GDREGISTER_CLASS(ModifyAndroidBuildTemplatePlugin);
		return;
	}
	
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(FmodChannelControl);
	GDREGISTER_CLASS(FmodChannel);
	GDREGISTER_CLASS(FmodChannelGroup);
	GDREGISTER_CLASS(FmodSoundGroup);
	GDREGISTER_CLASS(FmodDSPConnection);
	GDREGISTER_CLASS(FmodDSP);
	GDREGISTER_CLASS(FmodSound);
	GDREGISTER_CLASS(FmodSoundLock);
	GDREGISTER_CLASS(FmodSystem);

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

	GDREGISTER_CLASS(FmodAudioBus);
	GDREGISTER_CLASS(FmodAudioBusLayout);

	GDREGISTER_CLASS(FmodAudioStream);
	GDREGISTER_CLASS(FmodAudioStreamFLAC);

	GDREGISTER_CLASS(FmodReverb3D);

	GDREGISTER_CLASS(FmodGeometry);

	GDREGISTER_CLASS(FmodAudioStreamPlayer);
	GDREGISTER_CLASS(FmodAudioStreamPlayer2D);
	GDREGISTER_CLASS(FmodAudioStreamPlayer3D);
	GDREGISTER_CLASS(FmodGeometryInstance3D);

	GDREGISTER_CLASS(FmodServer);
	fmod_server_instance = memnew(FmodServer);
	Engine::get_singleton()->register_singleton("FmodServer", FmodServer::get_singleton());
}

void uninitialize_fmod_player_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_EDITOR) {
		return;
	}

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

		init_obj.register_initializer(initialize_fmod_player_module);
		init_obj.register_terminator(uninitialize_fmod_player_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);

		return init_obj.init();
	}
}
