#include "audio/fmod_audio_stream.h"

#include <godot_cpp/classes/file_access.hpp>

namespace godot {
	void FmodAudioStream::_bind_methods() {
		BIND_ENUM_CONSTANT(MODE_DEFAULT);
		BIND_ENUM_CONSTANT(MODE_STREAM);
		BIND_ENUM_CONSTANT(MODE_SAMPLE);
		BIND_ENUM_CONSTANT(MODE_LOOP);
		BIND_ENUM_CONSTANT(MODE_LOOP_BIDI);

		ClassDB::bind_method(D_METHOD("set_audio_data", "data"), &FmodAudioStream::set_audio_data);
		ClassDB::bind_method(D_METHOD("get_audio_data"), &FmodAudioStream::get_audio_data);
		ADD_PROPERTY(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "audio_data", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE), "set_audio_data", "get_audio_data");

		ClassDB::bind_method(D_METHOD("set_mode_flags", "flags"), &FmodAudioStream::set_mode_flags);
		ClassDB::bind_method(D_METHOD("get_mode_flags"), &FmodAudioStream::get_mode_flags);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "mode_flags", PROPERTY_HINT_FLAGS, "Stream,Sample,Loop,Loop Bidi"), "set_mode_flags", "get_mode_flags");

		ClassDB::bind_method(D_METHOD("get_sound"), &FmodAudioStream::get_sound);
		ClassDB::bind_method(D_METHOD("get_length"), &FmodAudioStream::get_length);
		ClassDB::bind_method(D_METHOD("is_data_loaded"), &FmodAudioStream::is_data_loaded);
		ClassDB::bind_method(D_METHOD("clear"), &FmodAudioStream::clear);

		ClassDB::bind_static_method("FmodAudioStream", D_METHOD("load_from_file", "path", "flags"), &FmodAudioStream::load_from_file, DEFVAL(MODE_STREAM));
	}

	FmodAudioStream::FmodAudioStream() {
		create_mode_flags = MODE_STREAM;
	}

	FmodAudioStream::~FmodAudioStream() {
		clear();
	}

	void FmodAudioStream::set_audio_data(const PackedByteArray& p_data) {
		audio_data = p_data;
		data_loaded = !audio_data.is_empty();
	}

	PackedByteArray FmodAudioStream::get_audio_data() const {
		return audio_data;
	}

	void FmodAudioStream::set_mode_flags(int p_flags) {
		create_mode_flags = p_flags;
		// 清理现有 sound，下次 get_sound() 会使用新标志重新创建
		if (sound.is_valid()) {
			sound.unref();
		}
	}

	int FmodAudioStream::get_mode_flags() const {
		return create_mode_flags;
	}

	void FmodAudioStream::add_mode_flag(CreateMode p_flag) {
		create_mode_flags |= p_flag;
	}

	void FmodAudioStream::remove_mode_flag(CreateMode p_flag) {
		create_mode_flags &= ~p_flag;
	}

	bool FmodAudioStream::has_mode_flag(CreateMode p_flag) const {
		return (create_mode_flags & p_flag) != 0;
	}

	Ref<FmodSound> FmodAudioStream::_create_sound() {
		if (!data_loaded || audio_data.is_empty()) {
			return Ref<FmodSound>();
		}

		Ref<FmodSystem> system = FmodServer::get_main_system();
		if (system.is_null() || system->system_is_null()) {
			return Ref<FmodSound>();
		}

		// 转换创建标志为 FMOD 标志
		unsigned int fmod_mode = FmodSystem::FMOD_MODE_OPENMEMORY;

		if (has_mode_flag(MODE_STREAM)) {
			fmod_mode |= FmodSystem::FMOD_MODE_CREATESTREAM;
		}
		else {
			fmod_mode |= FmodSystem::FMOD_MODE_CREATESAMPLE | FmodSystem::FMOD_MODE_LOOP_OFF;
		}

		if (has_mode_flag(MODE_LOOP)) {
			fmod_mode |= FmodSystem::FMOD_MODE_LOOP_NORMAL;
		}
		if (has_mode_flag(MODE_LOOP_BIDI)) {
			fmod_mode |= FmodSystem::FMOD_MODE_LOOP_BIDI;
		}

		// 处理 2D/3D 模式（默认 2D）
		if (has_mode_flag(MODE_3D)) {
			fmod_mode |= FmodSystem::FMOD_MODE_3D;
		}
		else {
			// 默认使用 2D 模式
			fmod_mode |= FmodSystem::FMOD_MODE_2D;
		}

		return system->create_sound_from_memory(audio_data, fmod_mode);
	}

	Ref<FmodSound> FmodAudioStream::get_sound() {
		if (sound.is_valid()) {
			return sound;
		}

		if (data_loaded && !audio_data.is_empty()) {
			sound = _create_sound();
		}

		return sound;
	}

	double FmodAudioStream::get_length() const {
		if (sound.is_valid()) {
			return sound->get_length();
		}
		return 0.0;
	}

	bool FmodAudioStream::is_data_loaded() const {
		return data_loaded;
	}

	Ref<FmodAudioStream> FmodAudioStream::load_from_file(const String& p_path, int p_flags) {
		Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::READ);
		ERR_FAIL_COND_V_MSG(file.is_null(), Ref<FmodAudioStream>(), "Cannot open audio file: " + p_path);
		
		PackedByteArray data = file->get_buffer(file->get_length());
		file->close();
		ERR_FAIL_COND_V_MSG(data.is_empty(), Ref<FmodAudioStream>(), "Failed to read audio file: " + p_path);

		Ref<FmodAudioStream> stream;
		stream.instantiate();
		stream->set_audio_data(data);
		stream->set_mode_flags(p_flags);

		return stream;
	}

	void FmodAudioStream::clear() {
		if (sound.is_valid()) {
			sound.unref();
		}
		audio_data.clear();
		data_loaded = false;
	}

	void FmodAudioStream::invalidate_sound() {
		if (sound.is_valid()) {
			sound.unref();
		}
	}
}
