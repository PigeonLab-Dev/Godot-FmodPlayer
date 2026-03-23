#include "nodes/fmod_audio_stream_player.h"
#include "mixer/fmod_audio_bus_layout.h"
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/editor_interface.hpp>

namespace godot {
	void FmodAudioStreamPlayer::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_stream", "stream"), &FmodAudioStreamPlayer::set_stream);
		ClassDB::bind_method(D_METHOD("get_stream"), &FmodAudioStreamPlayer::get_stream);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "stream", PROPERTY_HINT_RESOURCE_TYPE, "FmodAudioStream"), "set_stream", "get_stream");
		
		ClassDB::bind_method(D_METHOD("play", "from_position"), &FmodAudioStreamPlayer::play, DEFVAL(0.0f));
		ClassDB::bind_method(D_METHOD("seek", "to_position"), &FmodAudioStreamPlayer::play);
		ClassDB::bind_method(D_METHOD("stop"), &FmodAudioStreamPlayer::stop);

		ClassDB::bind_method(D_METHOD("set_playing", "playing"), &FmodAudioStreamPlayer::set_playing);
		ClassDB::bind_method(D_METHOD("is_playing"), &FmodAudioStreamPlayer::is_playing);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "set_playing", "is_playing");

		ClassDB::bind_method(D_METHOD("set_stream_paused", "paused"), &FmodAudioStreamPlayer::set_stream_paused);
		ClassDB::bind_method(D_METHOD("get_stream_paused"), &FmodAudioStreamPlayer::get_stream_paused);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "stream_paused"), "set_stream_paused", "get_stream_paused");

		ClassDB::bind_method(D_METHOD("get_playback_position"), &FmodAudioStreamPlayer::get_playback_position);

		ClassDB::bind_method(D_METHOD("set_volume_db", "volume_db"), &FmodAudioStreamPlayer::set_volume_db);
		ClassDB::bind_method(D_METHOD("get_volume_db"), &FmodAudioStreamPlayer::get_volume_db);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume_db", PROPERTY_HINT_RANGE, "-80,24,0.1,suffix:dB"), "set_volume_db", "get_volume_db");
		
		ClassDB::bind_method(D_METHOD("set_pitch_scale", "pitch_scale"), &FmodAudioStreamPlayer::set_pitch_scale);
		ClassDB::bind_method(D_METHOD("get_pitch_scale"), &FmodAudioStreamPlayer::get_pitch_scale);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch_scale", PROPERTY_HINT_RANGE, "0.01,4,0.01,or_greater"), "set_pitch_scale", "get_pitch_scale");

		ClassDB::bind_method(D_METHOD("set_auto_play", "enable"), &FmodAudioStreamPlayer::set_auto_play);
		ClassDB::bind_method(D_METHOD("is_autoplay_enabled"), &FmodAudioStreamPlayer::is_autoplay_enabled);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_play"), "set_auto_play", "is_autoplay_enabled");

		ClassDB::bind_method(D_METHOD("set_bus", "bus"), &FmodAudioStreamPlayer::set_bus);
		ClassDB::bind_method(D_METHOD("get_bus"), &FmodAudioStreamPlayer::get_bus);
		ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "bus", PROPERTY_HINT_ENUM), "set_bus", "get_bus");

		ADD_SIGNAL(MethodInfo("finished"));
	}

	FmodAudioStreamPlayer::FmodAudioStreamPlayer() {
		internal_channel.instantiate();
		if (FmodServer::get_singleton()) {
			Ref<FmodSystem> system = FmodServer::get_main_system();
			if (system.is_valid()) {
				internal_channel_group = system->get_master_channel_group();
			}
		}
	}

	void FmodAudioStreamPlayer::_notification(int p_what) {
		switch (p_what) {
		case NOTIFICATION_READY: {
			if (auto_play && stream.is_valid() && !Engine::get_singleton()->is_editor_hint()) {
				call_deferred("play");
			}
		} break;

		case NOTIFICATION_PREDELETE: {
			if (internal_channel.is_valid()) {
				internal_channel->stop();
				internal_channel.unref();
			}
			if (internal_channel_group.is_valid()) {
				internal_channel_group.unref();
			}
		} break;
		}
	}

	void FmodAudioStreamPlayer::_validate_property(PropertyInfo& p_property) const {
		if (p_property.name == StringName("bus")) {
			// 动态构建枚举字符串
			String bus_list;
			AudioServer* audio_server = AudioServer::get_singleton();

			if (audio_server) {
				int bus_count = audio_server->get_bus_count();
				for (int i = 0; i < bus_count; i++) {
					if (i > 0) bus_list += ",";
					bus_list += audio_server->get_bus_name(i);
				}
			}

			p_property.hint = PROPERTY_HINT_ENUM;
			p_property.hint_string = bus_list;
		}
	}

	void FmodAudioStreamPlayer::_create_internal_channel(Ref<FmodAudioStream> stream) {
		Ref<FmodSystem> system = FmodServer::get_main_system();

		// 通过 get_sound() 获取 Sound (延迟创建)
		Ref<FmodSound> sound = stream->get_sound();
		if (sound.is_null()) {
			UtilityFunctions::push_error("Failed to get sound from stream");
			return;
		}

		internal_channel = system->play_sound(sound, internal_channel_group, true);
		if (internal_channel.is_null()) {
			UtilityFunctions::push_error("Failed to get Fmod Channel!");
			return;
		}

		internal_channel->set_volume_db(volume_db);
		internal_channel->set_pitch(pitch);
		internal_channel->connect("ended", callable_mp(this, &FmodAudioStreamPlayer::_on_internal_channel_ended), CONNECT_DEFERRED);
	}

	void FmodAudioStreamPlayer::_on_internal_channel_ended() {
		if (internal_channel.is_valid()) {
			internal_channel->disconnect("ended", callable_mp(this, &FmodAudioStreamPlayer::_on_internal_channel_ended));
		}
		internal_channel.unref();

		// stop 会提前设置 playing 为 false，所以判断是否还在播放就行
		if (playing) {
			playing = false;
			emit_signal("finished");
		}
	}

	void FmodAudioStreamPlayer::set_stream(Ref<FmodAudioStream> new_stream) {
		stream = new_stream;

		// 停止播放音频
		if (internal_channel.is_valid() && internal_channel->is_playing()) {
			internal_channel->stop();
		}
		playing = false;
	}

	Ref<FmodAudioStream> FmodAudioStreamPlayer::get_stream() const {
		return stream;
	}

	void FmodAudioStreamPlayer::play(const float from_position) {
		// 延迟设置音频总线
		StringName actual_bus = get_bus();
		Ref<FmodAudioBus> audio_bus = FmodServer::get_audio_bus_layout()->get_audio_bus(actual_bus);
		
		Ref<FmodChannelGroup> channel_group;
		if (audio_bus.is_null()) {
			// 如果找不到总线 (可能还没同步)，直接回退到 FMOD Master ChannelGroup
			channel_group = FmodServer::get_master_channel_group();
		} else {
			channel_group = audio_bus->get_bus();
		}
		
		if (channel_group.is_null()) {
			UtilityFunctions::push_error(vformat("Cannot get channel group for bus: %s", actual_bus));
			return;
		}
		internal_channel_group = channel_group;

		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			// 如果已有 channel，尝试使用它
			internal_channel->set_position(int(from_position * 1000));
			internal_channel->set_paused(false);
			playing = true;
			return;
		}

		if (stream.is_valid()) {
			Ref<FmodSound> current_sound = stream->get_sound();
			if (current_sound.is_null()) {
				UtilityFunctions::push_error("No sound available");
				return;
			}

			_create_internal_channel(stream);
			if (internal_channel.is_valid()) {
				if (from_position > 0.0) {
					internal_channel->set_position(int(from_position * 1000));
				}
				internal_channel->set_paused(false);
				playing = true;
			}
		}
	}

	void FmodAudioStreamPlayer::seek(const float to_position) {
		ERR_FAIL_COND(!internal_channel.is_valid() || !internal_channel->channel_is_valid());
		internal_channel->set_position(int(to_position * 1000));
	}

	void FmodAudioStreamPlayer::stop() {
		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			playing = false;
			stream_paused = false;
			internal_channel->stop();
		}
	}

	void FmodAudioStreamPlayer::set_playing(const bool playing) {
		if (playing) {
			play();
		}
		else {
			stop();
		}
	}

	bool FmodAudioStreamPlayer::is_playing() const {
		if (internal_channel.is_null() || internal_channel->channel_is_null()) return false;
		return playing && internal_channel->is_playing();
	}

	void FmodAudioStreamPlayer::set_stream_paused(const bool paused) {
		stream_paused = paused;
		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			internal_channel->set_paused(paused);
		}
	}

	bool FmodAudioStreamPlayer::get_stream_paused() const {
		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			return internal_channel->get_paused();
		}
		return stream_paused;
	}

	float FmodAudioStreamPlayer::get_playback_position() const {
		if (internal_channel.is_null() || internal_channel->channel_is_null()) return 0.0;
		return internal_channel->get_position() / 1000.0;
	}

	void FmodAudioStreamPlayer::set_volume_db(const float new_volume_db) {
		volume_db = new_volume_db;

		if (internal_channel.is_null() || internal_channel->channel_is_null()) return;
		internal_channel->set_volume_db(volume_db);
	}

	float FmodAudioStreamPlayer::get_volume_db() const {
		return volume_db;
	}

	void FmodAudioStreamPlayer::set_pitch_scale(const float new_pitch) {
		pitch = new_pitch;

		if (internal_channel.is_null() || internal_channel->channel_is_null()) return;
		internal_channel->set_pitch(pitch);
	}

	float FmodAudioStreamPlayer::get_pitch_scale() const {
		return pitch;
	}

	void FmodAudioStreamPlayer::set_auto_play(const bool enable) {
		auto_play = enable;
	}

	bool FmodAudioStreamPlayer::is_autoplay_enabled() const {
		return auto_play;
	}

	void FmodAudioStreamPlayer::set_bus(const StringName& p_bus) {
		bus = p_bus;
	}

	StringName FmodAudioStreamPlayer::get_bus() const {
		const String bus_name = bus;
		for (int i = 0; i < AudioServer::get_singleton()->get_bus_count(); i++) {
			if (AudioServer::get_singleton()->get_bus_name(i) == bus_name) {
				return bus;
			}
		}
		return StringName("Master");
	}
}
