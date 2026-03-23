#ifndef FMOD_AUDIO_STREAM_PLAYER_H
#define FMOD_AUDIO_STREAM_PLAYER_H

#include "audio/fmod_audio_stream.h"
#include "playback/fmod_channel_group.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {
	class FmodAudioStreamPlayer : public Node {
		GDCLASS(FmodAudioStreamPlayer, Node)

	private:
		Ref<FmodChannel> internal_channel = nullptr;
		Ref<FmodChannelGroup> internal_channel_group = nullptr;

		void _create_internal_channel(Ref<FmodAudioStream> stream);
		void _on_internal_channel_ended();

	protected:
		static void _bind_methods();
		void _validate_property(PropertyInfo& p_property) const;

	public:
		FmodAudioStreamPlayer();
		
		void _notification(int p_what);

		Ref<FmodAudioStream> stream;
		bool playing = false;
		bool stream_paused = false;
		float volume_db = 0.0;
		float pitch = 1.0;
		bool auto_play = false;
		StringName bus;

		void set_stream(Ref<FmodAudioStream> new_stream);
		Ref<FmodAudioStream> get_stream() const;

		void play(const float from_position = 0.0);
		void seek(const float to_position);
		void stop();

		void set_playing(const bool playing);
		bool is_playing() const;

		void set_stream_paused(const bool paused);
		bool get_stream_paused() const;

		float get_playback_position() const;

		void set_volume_db(const float new_volume_db);
		float get_volume_db() const;

		void set_pitch_scale(const float new_pitch);
		float get_pitch_scale() const;

		void set_auto_play(const bool enable);
		bool is_autoplay_enabled() const;

		void set_bus(const StringName& p_bus);
		StringName get_bus() const;
	};
}

#endif // !FMOD_AUDIO_STREAM_PLAYER_H