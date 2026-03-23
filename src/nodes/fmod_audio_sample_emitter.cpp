#include "nodes/fmod_audio_sample_emitter.h"

namespace godot {

void FmodAudioSampleEmitter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("emit"), &FmodAudioSampleEmitter::emit);

	ClassDB::bind_method(D_METHOD("set_stream", "stream"), &FmodAudioSampleEmitter::set_stream);
	ClassDB::bind_method(D_METHOD("get_stream"), &FmodAudioSampleEmitter::get_stream);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "stream", PROPERTY_HINT_RESOURCE_TYPE, "FmodAudioStream"), "set_stream", "get_stream");
	
	ClassDB::bind_method(D_METHOD("set_auto_emit", "enable"), &FmodAudioSampleEmitter::set_auto_emit);
	ClassDB::bind_method(D_METHOD("is_autoemit_enabled"), &FmodAudioSampleEmitter::is_autoemit_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_emit"), "set_auto_emit", "is_autoemit_enabled");
}

FmodAudioSampleEmitter::FmodAudioSampleEmitter() {
	Ref<FmodSystem> system = FmodServer::get_main_system();
	if (system.is_valid()) {
		internal_channel_group = system->get_master_channel_group();
	}
}

void FmodAudioSampleEmitter::_notification(int p_what) {
	switch (p_what) {
	case NOTIFICATION_READY: {
		if (auto_emit && stream.is_valid() && !Engine::get_singleton()->is_editor_hint()) {
			call("emit");
		}
	} break;

	case NOTIFICATION_PREDELETE: {
		if (internal_channel_group.is_valid()) {
			internal_channel_group.unref();
		}
	} break;
	}
}

void FmodAudioSampleEmitter::emit() {
	ERR_FAIL_COND(stream.is_null() || internal_channel_group.is_null());
	
	// 发射器默认使用样本模式（一次性播放，加载到内存）
	if (!stream->has_mode_flag(FmodAudioStream::MODE_SAMPLE)) {
		stream->add_mode_flag(FmodAudioStream::MODE_SAMPLE);
		// 移除流式标志（如果有的话）
		stream->remove_mode_flag(FmodAudioStream::MODE_STREAM);
	}
	
	Ref<FmodSound> sound = stream->get_sound();
	if (sound.is_null()) {
		UtilityFunctions::push_error("Failed to get sound from stream");
		return;
	}
	
	Ref<FmodSystem> system = FmodServer::get_main_system();
	if (system.is_null()) {
		UtilityFunctions::push_error("FMOD system not available");
		return;
	}
	
	Ref<FmodChannel> channel = system->play_sound(sound, internal_channel_group);
	if (channel.is_null()) {
		UtilityFunctions::push_error("Failed to play sound");
	}
}

void FmodAudioSampleEmitter::set_stream(Ref<FmodAudioStream> new_stream) {
	stream = new_stream;
}

Ref<FmodAudioStream> FmodAudioSampleEmitter::get_stream() const {
	return stream;
}

void FmodAudioSampleEmitter::set_auto_emit(const bool enable) {
	auto_emit = enable;
}

bool FmodAudioSampleEmitter::is_autoemit_enabled() const {
	return auto_emit;
}

} // namespace godot
