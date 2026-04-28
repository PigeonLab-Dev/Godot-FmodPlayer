#include "fmod_audio_importer.h"
#include "audio/fmod_audio_stream.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

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

		// Keep this list aligned with FMOD_SOUND_TYPE formats that can be
		// reasonably detected from normal files or memory-backed imports.
		static const char* recognized_extensions[] = {
			"aac",
			"adts",
			"aif",
			"aifc",
			"aiff",
			"asf",
			"at9",
			"caf",
			"flac",
			"fsb",
			"it",
			"m4a",
			"mid",
			"midi",
			"mod",
			"mp2",
			"mp3",
			"mp4",
			"oga",
			"ogg",
			"opus",
			"rmi",
			"s3m",
			"wav",
			"wma",
			"wmv",
			"xma",
			"xm",
			"3gp",
		};

		for (const char* extension : recognized_extensions) {
			extensions.append(extension);
		}

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
			case 0:
				return "Stream (Low memory)";
			case 1:
				return "Sample (High quality)";
			case 2:
				return "Loop Sample";
			default:
				return "Default";
		}
	}

	TypedArray<Dictionary> AudioImporterFmod::_get_import_options(const String& p_path, int32_t p_preset_index) const {
		TypedArray<Dictionary> options;

		int default_flags = FmodAudioStream::MODE_STREAM;
		switch (p_preset_index) {
			case 0:
				default_flags = FmodAudioStream::MODE_STREAM;
				break;
			case 1:
				default_flags = FmodAudioStream::MODE_SAMPLE;
				break;
			case 2:
				default_flags = FmodAudioStream::MODE_SAMPLE | FmodAudioStream::MODE_LOOP;
				break;
		}

		Dictionary flags_option;
		flags_option["name"] = "flags";
		flags_option["default_value"] = default_flags;
		flags_option["property_hint"] = PROPERTY_HINT_FLAGS;
		flags_option["hint_string"] = "Stream,Sample,Loop,Loop Bidi,2D,3D";
		options.append(flags_option);

		return options;
	}

	bool AudioImporterFmod::_get_option_visibility(const String& p_path, const StringName& p_option_name, const Dictionary& p_options) const {
		return true;
	}

	Error AudioImporterFmod::_import(const String& p_source_file, const String& p_save_path, const Dictionary& p_options, const TypedArray<String>& p_platform_variants, const TypedArray<String>& p_gen_files) const {
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

		Ref<FmodAudioStream> stream;
		stream.instantiate();
		stream->set_audio_data(audio_data);

		if (p_options.has("flags")) {
			int flags = p_options["flags"];
			stream->set_mode_flags(flags);
		}
		else {
			stream->set_mode_flags(FmodAudioStream::MODE_STREAM);
		}

		String save_path = p_save_path + String(".") + _get_save_extension();
		Error err = ResourceSaver::get_singleton()->save(stream, save_path);
		if (err != OK) {
			ERR_PRINT("Failed to save FMOD audio resource: " + save_path + ", error: " + String::num_int64(err));
			return err;
		}

		return OK;
	}
}
