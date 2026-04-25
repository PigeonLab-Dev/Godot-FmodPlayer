#ifndef FMOD_AUDIO_STREAM_PLAYER_2D_H
#define FMOD_AUDIO_STREAM_PLAYER_2D_H

#include "audio/fmod_audio_stream.h"
#include "core/fmod_server.h"
#include "nodes/fmod_audio_stream_player_internal.h"
#include "playback/fmod_channel.h"
#include <godot_cpp/classes/camera2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/physics_direct_space_state2d.hpp>
#include <godot_cpp/classes/physics_point_query_parameters2d.hpp>
#include <godot_cpp/classes/physics_server2d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/world2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
	class FmodAudioStreamPlayer2D : public Node2D {
		GDCLASS(FmodAudioStreamPlayer2D, Node2D)

	private:
		FmodAudioStreamPlayerInternal* internal_player = nullptr;

		float max_distance = 2000.0f;
		float attenuation = 1.0f;
		float panning_strength = 1.0f;
		uint32_t area_mask = 1;
		int max_polyphony = 1;

		float setplay = -1.0f;
		uint64_t last_mix_count = 0;
		bool force_update_panning = false;
		Vector2 last_position;

		void _on_internal_player_finished();
		void _update_panning();

		static void _listener_changed_callback(void *self) {
			reinterpret_cast<FmodAudioStreamPlayer2D *>(self)->force_update_panning = true;
		}

	protected:
		static void _bind_methods();
		void _notification(int p_what);
		void _validate_property(PropertyInfo& p_property) const;

	public:
		FmodAudioStreamPlayer2D();
		~FmodAudioStreamPlayer2D();

		void set_stream(Ref<FmodAudioStream> new_stream);
		Ref<FmodAudioStream> get_stream() const;

		bool preload_stream();

		void play(const float from_position = 0.0f);
		void seek(const float to_position);
		void stop();

		void set_playing(const bool playing);
		bool is_playing() const;

		void set_stream_paused(const bool paused);
		bool get_stream_paused() const;

		float get_playback_position() const;

		void set_volume_db(const float new_volume_db);
		float get_volume_db() const;

		void set_volume_linear(const float p_volume);
		float get_volume_linear() const;

		void set_pitch_scale(const float new_pitch);
		float get_pitch_scale() const;

		void set_autoplay(const bool enable);
		bool is_autoplay_enabled() const;

		void set_preload_on_set_stream(const bool enable);
		bool is_preload_on_set_stream_enabled() const;

		void set_max_distance(float p_pixels);
		float get_max_distance() const;

		void set_attenuation(float p_curve);
		float get_attenuation() const;

		void set_panning_strength(float p_panning_strength);
		float get_panning_strength() const;

		void set_bus(const StringName& p_bus);
		StringName get_bus() const;

		void set_area_mask(uint32_t p_mask);
		uint32_t get_area_mask() const;

		void set_max_polyphony(int p_max_polyphony);
		int get_max_polyphony() const;

		bool has_stream_playback() const;
		Ref<FmodChannel> get_stream_playback() const;
	};
}

#endif // !FMOD_AUDIO_STREAM_PLAYER_2D_H
