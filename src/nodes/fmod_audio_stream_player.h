#ifndef FMOD_AUDIO_STREAM_PLAYER_H
#define FMOD_AUDIO_STREAM_PLAYER_H

#include "audio/fmod_audio_stream.h"
#include "nodes/fmod_audio_stream_player_internal.h"
#include <godot_cpp/classes/node.hpp>

namespace godot {
	class FmodAudioStreamPlayer : public Node {
		GDCLASS(FmodAudioStreamPlayer, Node)

	private:
		FmodAudioStreamPlayerInternal* internal_player = nullptr;
		void _on_internal_player_finished();

	protected:
		static void _bind_methods();
		void _validate_property(PropertyInfo& p_property) const;

	public:
		FmodAudioStreamPlayer();
		
		void _notification(int p_what);

		void set_stream(Ref<FmodAudioStream> new_stream);
		Ref<FmodAudioStream> get_stream() const;

		bool preload_stream();

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

		void set_preload_on_set_stream(const bool enable);
		bool is_preload_on_set_stream_enabled() const;

		void set_bus(const StringName& p_bus);
		StringName get_bus() const;
	};
}

#endif // !FMOD_AUDIO_STREAM_PLAYER_H
