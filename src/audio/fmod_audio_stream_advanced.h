#ifndef FMOD_AUDIO_STREAM_ADVANCED_H
#define FMOD_AUDIO_STREAM_ADVANCED_H

#include "audio/fmod_audio_stream.h"
#include "audio/fmod_sound.h"

#include <godot_cpp/variant/packed_int32_array.hpp>

namespace godot {
	class FmodAudioStreamAdvanced : public FmodAudioStream {
		GDCLASS(FmodAudioStreamAdvanced, FmodAudioStream)

	private:
		int length_override = 0;
		int file_offset = 0;
		int decode_buffer_size = 0;
		int initial_subsound = -1;
		int num_subsounds = 0;
		PackedInt32Array inclusion_list;
		FmodSound::FmodSoundType suggested_sound_type = FmodSound::FMOD_SOUND_TYPE_UNKNOWN;
		int default_frequency = 0;
		int num_channels = 0;
		FmodSound::FmodSoundFormat format = FmodSound::FMOD_SOUND_FORMAT_NONE;
		String dls_name;
		String encryption_key;
		int max_polyphony = 0;
		int initial_seek_position = 0;
		FmodSystem::FmodTimeUnit initial_seek_postype = FmodSystem::FMOD_TIME_UNIT_MS;
		bool ignore_set_file_system = false;

		void _invalidate_on_change();

	protected:
		static void _bind_methods();
		virtual Ref<FmodSound> _create_sound() override;

	public:
		FmodAudioStreamAdvanced();
		virtual ~FmodAudioStreamAdvanced();

		void set_length_override(int p_length);
		int get_length_override() const;

		void set_file_offset(int p_offset);
		int get_file_offset() const;

		void set_decode_buffer_size(int p_size);
		int get_decode_buffer_size() const;

		void set_initial_subsound(int p_subsound);
		int get_initial_subsound() const;

		void set_num_subsounds(int p_count);
		int get_num_subsounds() const;

		void set_inclusion_list(const PackedInt32Array& p_list);
		PackedInt32Array get_inclusion_list() const;

		void set_suggested_sound_type(FmodSound::FmodSoundType p_type);
		FmodSound::FmodSoundType get_suggested_sound_type() const;

		void set_default_frequency(int p_frequency);
		int get_default_frequency() const;

		void set_num_channels(int p_channels);
		int get_num_channels() const;

		void set_format(FmodSound::FmodSoundFormat p_format);
		FmodSound::FmodSoundFormat get_format() const;

		void set_dls_name(const String& p_name);
		String get_dls_name() const;

		void set_encryption_key(const String& p_key);
		String get_encryption_key() const;

		void set_max_polyphony(int p_polyphony);
		int get_max_polyphony() const;

		void set_initial_seek_position(int p_position);
		int get_initial_seek_position() const;

		void set_initial_seek_postype(FmodSystem::FmodTimeUnit p_postype);
		FmodSystem::FmodTimeUnit get_initial_seek_postype() const;

		void set_ignore_set_file_system(bool p_ignore);
		bool is_ignore_set_file_system() const;

		static Ref<FmodAudioStreamAdvanced> load_from_file(const String& p_path, int p_flags = MODE_STREAM);
	};
}

#endif // !FMOD_AUDIO_STREAM_ADVANCED_H
