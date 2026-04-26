#include "audio/fmod_audio_stream_advanced.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <vector>

namespace godot {
	void FmodAudioStreamAdvanced::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_length_override", "length"), &FmodAudioStreamAdvanced::set_length_override);
		ClassDB::bind_method(D_METHOD("get_length_override"), &FmodAudioStreamAdvanced::get_length_override);
		ClassDB::bind_method(D_METHOD("set_file_offset", "offset"), &FmodAudioStreamAdvanced::set_file_offset);
		ClassDB::bind_method(D_METHOD("get_file_offset"), &FmodAudioStreamAdvanced::get_file_offset);
		ClassDB::bind_method(D_METHOD("set_decode_buffer_size", "size"), &FmodAudioStreamAdvanced::set_decode_buffer_size);
		ClassDB::bind_method(D_METHOD("get_decode_buffer_size"), &FmodAudioStreamAdvanced::get_decode_buffer_size);
		ClassDB::bind_method(D_METHOD("set_initial_subsound", "subsound"), &FmodAudioStreamAdvanced::set_initial_subsound);
		ClassDB::bind_method(D_METHOD("get_initial_subsound"), &FmodAudioStreamAdvanced::get_initial_subsound);
		ClassDB::bind_method(D_METHOD("set_num_subsounds", "count"), &FmodAudioStreamAdvanced::set_num_subsounds);
		ClassDB::bind_method(D_METHOD("get_num_subsounds"), &FmodAudioStreamAdvanced::get_num_subsounds);
		ClassDB::bind_method(D_METHOD("set_inclusion_list", "list"), &FmodAudioStreamAdvanced::set_inclusion_list);
		ClassDB::bind_method(D_METHOD("get_inclusion_list"), &FmodAudioStreamAdvanced::get_inclusion_list);
		ClassDB::bind_method(D_METHOD("set_suggested_sound_type", "type"), &FmodAudioStreamAdvanced::set_suggested_sound_type);
		ClassDB::bind_method(D_METHOD("get_suggested_sound_type"), &FmodAudioStreamAdvanced::get_suggested_sound_type);
		ClassDB::bind_method(D_METHOD("set_default_frequency", "frequency"), &FmodAudioStreamAdvanced::set_default_frequency);
		ClassDB::bind_method(D_METHOD("get_default_frequency"), &FmodAudioStreamAdvanced::get_default_frequency);
		ClassDB::bind_method(D_METHOD("set_num_channels", "channels"), &FmodAudioStreamAdvanced::set_num_channels);
		ClassDB::bind_method(D_METHOD("get_num_channels"), &FmodAudioStreamAdvanced::get_num_channels);
		ClassDB::bind_method(D_METHOD("set_format", "format"), &FmodAudioStreamAdvanced::set_format);
		ClassDB::bind_method(D_METHOD("get_format"), &FmodAudioStreamAdvanced::get_format);
		ClassDB::bind_method(D_METHOD("set_dls_name", "name"), &FmodAudioStreamAdvanced::set_dls_name);
		ClassDB::bind_method(D_METHOD("get_dls_name"), &FmodAudioStreamAdvanced::get_dls_name);
		ClassDB::bind_method(D_METHOD("set_encryption_key", "key"), &FmodAudioStreamAdvanced::set_encryption_key);
		ClassDB::bind_method(D_METHOD("get_encryption_key"), &FmodAudioStreamAdvanced::get_encryption_key);
		ClassDB::bind_method(D_METHOD("set_max_polyphony", "polyphony"), &FmodAudioStreamAdvanced::set_max_polyphony);
		ClassDB::bind_method(D_METHOD("get_max_polyphony"), &FmodAudioStreamAdvanced::get_max_polyphony);
		ClassDB::bind_method(D_METHOD("set_initial_seek_position", "position"), &FmodAudioStreamAdvanced::set_initial_seek_position);
		ClassDB::bind_method(D_METHOD("get_initial_seek_position"), &FmodAudioStreamAdvanced::get_initial_seek_position);
		ClassDB::bind_method(D_METHOD("set_initial_seek_postype", "postype"), &FmodAudioStreamAdvanced::set_initial_seek_postype);
		ClassDB::bind_method(D_METHOD("get_initial_seek_postype"), &FmodAudioStreamAdvanced::get_initial_seek_postype);
		ClassDB::bind_method(D_METHOD("set_ignore_set_file_system", "ignore"), &FmodAudioStreamAdvanced::set_ignore_set_file_system);
		ClassDB::bind_method(D_METHOD("is_ignore_set_file_system"), &FmodAudioStreamAdvanced::is_ignore_set_file_system);

		ClassDB::bind_static_method("FmodAudioStreamAdvanced", D_METHOD("load_from_file", "path", "flags"), &FmodAudioStreamAdvanced::load_from_file, DEFVAL(MODE_STREAM));

		ADD_PROPERTY(PropertyInfo(Variant::INT, "length_override", PROPERTY_HINT_RANGE, "0,2147483647,1,or_greater,suffix:bytes"), "set_length_override", "get_length_override");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "file_offset", PROPERTY_HINT_RANGE, "0,2147483647,1,or_greater,suffix:bytes"), "set_file_offset", "get_file_offset");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "decode_buffer_size", PROPERTY_HINT_RANGE, "0,1000000,1,or_greater,suffix:samples"), "set_decode_buffer_size", "get_decode_buffer_size");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "initial_subsound", PROPERTY_HINT_RANGE, "-1,65535,1"), "set_initial_subsound", "get_initial_subsound");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "num_subsounds", PROPERTY_HINT_RANGE, "0,65535,1"), "set_num_subsounds", "get_num_subsounds");
		ADD_PROPERTY(PropertyInfo(Variant::PACKED_INT32_ARRAY, "inclusion_list"), "set_inclusion_list", "get_inclusion_list");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "suggested_sound_type", PROPERTY_HINT_ENUM, "Unknown,AIFF,ASF,DLS,FLAC,FSB,IT,MIDI,MOD,MPEG,OGGVORBIS,PLAYLIST,RAW,S3M,USER,WAV,XM,XMA,AUDIOQUEUE,AT9,VORBIS,MEDIA_FOUNDATION,MEDIACODEC,FADPCM,OPUS,MAX"), "set_suggested_sound_type", "get_suggested_sound_type");

		ADD_GROUP("Raw PCM", "");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "default_frequency", PROPERTY_HINT_RANGE, "0,384000,1,or_greater,suffix:Hz"), "set_default_frequency", "get_default_frequency");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "num_channels", PROPERTY_HINT_RANGE, "0,32,1"), "set_num_channels", "get_num_channels");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "format", PROPERTY_HINT_ENUM, "None,PCM8,PCM16,PCM24,PCM32,PCMFloat,Bitstream,Max"), "set_format", "get_format");

		ADD_GROUP("Advanced", "");
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "dls_name", PROPERTY_HINT_FILE), "set_dls_name", "get_dls_name");
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "encryption_key"), "set_encryption_key", "get_encryption_key");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "max_polyphony", PROPERTY_HINT_RANGE, "0,4095,1,or_greater"), "set_max_polyphony", "get_max_polyphony");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "initial_seek_position", PROPERTY_HINT_RANGE, "0,2147483647,1,or_greater"), "set_initial_seek_position", "get_initial_seek_position");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "initial_seek_postype", PROPERTY_HINT_FLAGS, "MS,PCM,PCM Bytes,Raw Bytes,PCM Fraction,,,,MOD Order,MOD Row,MOD Pattern"), "set_initial_seek_postype", "get_initial_seek_postype");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "ignore_set_file_system"), "set_ignore_set_file_system", "is_ignore_set_file_system");
	}

	FmodAudioStreamAdvanced::FmodAudioStreamAdvanced() {
	}

	FmodAudioStreamAdvanced::~FmodAudioStreamAdvanced() {
	}

	void FmodAudioStreamAdvanced::_invalidate_on_change() {
		invalidate_sound();
	}

	Ref<FmodSound> FmodAudioStreamAdvanced::_create_sound() {
		if (!data_loaded || audio_data.is_empty()) {
			return Ref<FmodSound>();
		}

		Ref<FmodSystem> system = FmodServer::get_main_system();
		if (system.is_null() || system->system_is_null()) {
			return Ref<FmodSound>();
		}

		Ref<FmodSound> fmod_sound;
		fmod_sound.instantiate();
		fmod_sound->data = audio_data;

		FMOD_CREATESOUNDEXINFO exinfo = {};
		exinfo.cbsize = sizeof(exinfo);
		exinfo.length = length_override > 0 ? (unsigned int)MIN(length_override, audio_data.size()) : (unsigned int)audio_data.size();

		if (file_offset > 0) {
			exinfo.fileoffset = (unsigned int)file_offset;
		}
		if (decode_buffer_size > 0) {
			exinfo.decodebuffersize = (unsigned int)decode_buffer_size;
		}
		if (initial_subsound >= 0) {
			exinfo.initialsubsound = initial_subsound;
		}
		if (num_subsounds > 0) {
			exinfo.numsubsounds = num_subsounds;
		}

		std::vector<int> inclusion_storage;
		if (!inclusion_list.is_empty()) {
			inclusion_storage.reserve(inclusion_list.size());
			for (int i = 0; i < inclusion_list.size(); i++) {
				inclusion_storage.push_back(inclusion_list[i]);
			}
			exinfo.inclusionlist = inclusion_storage.data();
			exinfo.inclusionlistnum = (int)inclusion_storage.size();
		}

		if (suggested_sound_type != FmodSound::FMOD_SOUND_TYPE_UNKNOWN) {
			exinfo.suggestedsoundtype = (FMOD_SOUND_TYPE)suggested_sound_type;
		}
		if (default_frequency > 0) {
			exinfo.defaultfrequency = default_frequency;
		}
		if (num_channels > 0) {
			exinfo.numchannels = num_channels;
		}
		if (format != FmodSound::FMOD_SOUND_FORMAT_NONE) {
			exinfo.format = (FMOD_SOUND_FORMAT)format;
		}

		CharString dls_name_utf8;
		if (!dls_name.is_empty()) {
			String resolved_dls_name = dls_name;
			if (dls_name.begins_with("res://") || dls_name.begins_with("user://")) {
				resolved_dls_name = ProjectSettings::get_singleton()->globalize_path(dls_name);
			}
			dls_name_utf8 = resolved_dls_name.utf8();
			exinfo.dlsname = dls_name_utf8.get_data();
		}

		CharString encryption_key_utf8;
		if (!encryption_key.is_empty()) {
			encryption_key_utf8 = encryption_key.utf8();
			exinfo.encryptionkey = encryption_key_utf8.get_data();
		}

		if (max_polyphony > 0) {
			exinfo.maxpolyphony = max_polyphony;
		}
		if (initial_seek_position > 0) {
			exinfo.initialseekposition = (unsigned int)initial_seek_position;
			exinfo.initialseekpostype = (FMOD_TIMEUNIT)initial_seek_postype;
		}
		exinfo.ignoresetfilesystem = ignore_set_file_system ? 1 : 0;

		FMOD::Sound* sound_ptr = nullptr;
		FMOD_ERR_CHECK_V(system->get_system()->createSound(
			(const char*)fmod_sound->data.ptr(),
			_get_fmod_mode_flags(),
			&exinfo,
			&sound_ptr
		), Ref<FmodSound>());

		fmod_sound->setup(sound_ptr);
		return fmod_sound;
	}

	void FmodAudioStreamAdvanced::set_length_override(int p_length) {
		length_override = MAX(p_length, 0);
		_invalidate_on_change();
	}

	int FmodAudioStreamAdvanced::get_length_override() const {
		return length_override;
	}

	void FmodAudioStreamAdvanced::set_file_offset(int p_offset) {
		file_offset = MAX(p_offset, 0);
		_invalidate_on_change();
	}

	int FmodAudioStreamAdvanced::get_file_offset() const {
		return file_offset;
	}

	void FmodAudioStreamAdvanced::set_decode_buffer_size(int p_size) {
		decode_buffer_size = MAX(p_size, 0);
		_invalidate_on_change();
	}

	int FmodAudioStreamAdvanced::get_decode_buffer_size() const {
		return decode_buffer_size;
	}

	void FmodAudioStreamAdvanced::set_initial_subsound(int p_subsound) {
		initial_subsound = MAX(p_subsound, -1);
		_invalidate_on_change();
	}

	int FmodAudioStreamAdvanced::get_initial_subsound() const {
		return initial_subsound;
	}

	void FmodAudioStreamAdvanced::set_num_subsounds(int p_count) {
		num_subsounds = MAX(p_count, 0);
		_invalidate_on_change();
	}

	int FmodAudioStreamAdvanced::get_num_subsounds() const {
		return num_subsounds;
	}

	void FmodAudioStreamAdvanced::set_inclusion_list(const PackedInt32Array& p_list) {
		inclusion_list = p_list;
		_invalidate_on_change();
	}

	PackedInt32Array FmodAudioStreamAdvanced::get_inclusion_list() const {
		return inclusion_list;
	}

	void FmodAudioStreamAdvanced::set_suggested_sound_type(FmodSound::FmodSoundType p_type) {
		suggested_sound_type = p_type;
		_invalidate_on_change();
	}

	FmodSound::FmodSoundType FmodAudioStreamAdvanced::get_suggested_sound_type() const {
		return suggested_sound_type;
	}

	void FmodAudioStreamAdvanced::set_default_frequency(int p_frequency) {
		default_frequency = MAX(p_frequency, 0);
		_invalidate_on_change();
	}

	int FmodAudioStreamAdvanced::get_default_frequency() const {
		return default_frequency;
	}

	void FmodAudioStreamAdvanced::set_num_channels(int p_channels) {
		num_channels = CLAMP(p_channels, 0, 32);
		_invalidate_on_change();
	}

	int FmodAudioStreamAdvanced::get_num_channels() const {
		return num_channels;
	}

	void FmodAudioStreamAdvanced::set_format(FmodSound::FmodSoundFormat p_format) {
		format = p_format;
		_invalidate_on_change();
	}

	FmodSound::FmodSoundFormat FmodAudioStreamAdvanced::get_format() const {
		return format;
	}

	void FmodAudioStreamAdvanced::set_dls_name(const String& p_name) {
		dls_name = p_name;
		_invalidate_on_change();
	}

	String FmodAudioStreamAdvanced::get_dls_name() const {
		return dls_name;
	}

	void FmodAudioStreamAdvanced::set_encryption_key(const String& p_key) {
		encryption_key = p_key;
		_invalidate_on_change();
	}

	String FmodAudioStreamAdvanced::get_encryption_key() const {
		return encryption_key;
	}

	void FmodAudioStreamAdvanced::set_max_polyphony(int p_polyphony) {
		max_polyphony = MAX(p_polyphony, 0);
		_invalidate_on_change();
	}

	int FmodAudioStreamAdvanced::get_max_polyphony() const {
		return max_polyphony;
	}

	void FmodAudioStreamAdvanced::set_initial_seek_position(int p_position) {
		initial_seek_position = MAX(p_position, 0);
		_invalidate_on_change();
	}

	int FmodAudioStreamAdvanced::get_initial_seek_position() const {
		return initial_seek_position;
	}

	void FmodAudioStreamAdvanced::set_initial_seek_postype(FmodSystem::FmodTimeUnit p_postype) {
		initial_seek_postype = p_postype;
		_invalidate_on_change();
	}

	FmodSystem::FmodTimeUnit FmodAudioStreamAdvanced::get_initial_seek_postype() const {
		return initial_seek_postype;
	}

	void FmodAudioStreamAdvanced::set_ignore_set_file_system(bool p_ignore) {
		ignore_set_file_system = p_ignore;
		_invalidate_on_change();
	}

	bool FmodAudioStreamAdvanced::is_ignore_set_file_system() const {
		return ignore_set_file_system;
	}

	Ref<FmodAudioStreamAdvanced> FmodAudioStreamAdvanced::load_from_file(const String& p_path, int p_flags) {
		Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::READ);
		ERR_FAIL_COND_V_MSG(file.is_null(), Ref<FmodAudioStreamAdvanced>(), "Cannot open audio file: " + p_path);

		PackedByteArray data = file->get_buffer(file->get_length());
		file->close();
		ERR_FAIL_COND_V_MSG(data.is_empty(), Ref<FmodAudioStreamAdvanced>(), "Failed to read audio file: " + p_path);

		Ref<FmodAudioStreamAdvanced> stream;
		stream.instantiate();
		stream->set_audio_data(data);
		stream->set_mode_flags(p_flags);

		return stream;
	}
}
