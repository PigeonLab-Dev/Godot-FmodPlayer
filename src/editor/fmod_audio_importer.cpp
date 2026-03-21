#include "fmod_audio_importer.h"
#include "audio/fmod_audio_stream.h"
#include "audio/fmod_audio_stream_flac.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>

namespace godot {
	void AudioImporterFmod::_bind_methods() {

	}

	AudioImporterFmod::AudioImporterFmod() {

	}

	AudioImporterFmod::~AudioImporterFmod() {

	}

	String AudioImporterFmod::_get_importer_name() const {
		return "fmod_audio";
	}

	String AudioImporterFmod::_get_visible_name() const {
		return "FMOD Audio";
	}

	PackedStringArray AudioImporterFmod::_get_recognized_extensions() const {
		PackedStringArray extensions;
		extensions.append("flac");
		extensions.append("wav");
		extensions.append("mp3");
		extensions.append("ogg");
		extensions.append("aiff");
		extensions.append("wma");
		extensions.append("asf");
		extensions.append("xm");
		extensions.append("mod");
		extensions.append("s3m");
		extensions.append("it");
		return extensions;
	}

	String AudioImporterFmod::_get_save_extension() const {
		return "res";
	}

	String AudioImporterFmod::_get_resource_type() const {
		return "FmodAudioStream";
	}

	int32_t AudioImporterFmod::_get_preset_count() const {
		return 3;
	}

	String AudioImporterFmod::_get_preset_name(int32_t p_preset_index) const {
		switch (p_preset_index) {
		case 0: return "Stream (Low memory)";
		case 1: return "Sample (High quality)";
		case 2: return "Loop Sample";
		default: return "Default";
		}
	}

	TypedArray<Dictionary> AudioImporterFmod::_get_import_options(const String& p_path, int32_t p_preset_index) const {
		TypedArray<Dictionary> options;

		// 根据预设设置默认值
		int default_flags = FmodAudioStream::MODE_STREAM;
		switch (p_preset_index) {
		case 0: default_flags = FmodAudioStream::MODE_STREAM; break;
		case 1: default_flags = FmodAudioStream::MODE_SAMPLE; break;
		case 2: default_flags = FmodAudioStream::MODE_SAMPLE | FmodAudioStream::MODE_LOOP; break;
		}

		// 标志选项
		Dictionary flags_option;
		flags_option["name"] = "flags";
		flags_option["default_value"] = default_flags;
		flags_option["property_hint"] = PROPERTY_HINT_FLAGS;
		flags_option["hint_string"] = "Stream,Sample,Loop,Loop Bidi";
		options.append(flags_option);

		return options;
	}

	bool AudioImporterFmod::_get_option_visibility(const String& p_path, const StringName& p_option_name, const Dictionary& p_options) const {
		return true;
	}

	Error AudioImporterFmod::_import(const String& p_source_file, const String& p_save_path, const Dictionary& p_options, const TypedArray<String>& p_platform_variants, const TypedArray<String>& p_gen_files) const {
		// 读取源文件数据
		Ref<FileAccess> f = FileAccess::open(p_source_file, FileAccess::READ);
		if (f.is_null()) {
			ERR_PRINT("Cannot open audio file: " + p_source_file);
			return ERR_CANT_OPEN;
		}

		uint64_t file_size = f->get_length();
		PackedByteArray audio_data;
		audio_data.resize(file_size);
		f->get_buffer(audio_data.ptrw(), file_size);
		f->close();

		if (audio_data.is_empty()) {
			ERR_PRINT("Audio file is empty: " + p_source_file);
			return ERR_FILE_CORRUPT;
		}

		// 获取文件扩展名以确定资源类型
		String ext = p_source_file.get_extension().to_lower();

		Ref<FmodAudioStream> stream;

		// FLAC 使用专门的子类，其他格式使用基类
		if (ext == "flac") {
			Ref<FmodAudioStreamFLAC> flac_stream;
			flac_stream.instantiate();
			stream = flac_stream;
		}
		else {
			stream.instantiate();
		}

		stream->set_audio_data(audio_data);

		// 设置标志
		if (p_options.has("flags")) {
			int flags = p_options["flags"];
			stream->set_mode_flags(flags);
		}
		else {
			stream->set_mode_flags(FmodAudioStream::MODE_STREAM);
		}

		// 保存资源
		String save_path = p_save_path + String(".") + _get_save_extension();
		Error err = ResourceSaver::get_singleton()->save(stream, save_path);
		if (err != OK) {
			ERR_PRINT("Failed to save FMOD audio resource: " + save_path + ", error: " + String::num_int64(err));
			return err;
		}

		return OK;
	}
}
