#ifndef FMOD_AUDIO_IMPORTER_H
#define FMOD_AUDIO_IMPORTER_H

#include <godot_cpp/classes/editor_import_plugin.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>

namespace godot {
	class AudioImporterFmod : public EditorImportPlugin {
		GDCLASS(AudioImporterFmod, EditorImportPlugin)

	protected:
		static void _bind_methods();

	public:
		AudioImporterFmod();
		virtual ~AudioImporterFmod();

		virtual String _get_importer_name() const override;
		virtual String _get_visible_name() const override;
		virtual PackedStringArray _get_recognized_extensions() const override;
		virtual String _get_save_extension() const override;
		virtual String _get_resource_type() const override;

		virtual int32_t _get_preset_count() const override;
		virtual String _get_preset_name(int32_t p_preset_index) const override;
		virtual TypedArray<Dictionary> _get_import_options(const String& p_path, int32_t p_preset_index) const override;
		virtual bool _get_option_visibility(const String& p_path, const StringName& p_option_name, const Dictionary& p_options) const override;

		virtual Error _import(const String& p_source_file, const String& p_save_path, const Dictionary& p_options, const TypedArray<String>& p_platform_variants, const TypedArray<String>& p_gen_files) const override;
	};
}

#endif // !FMOD_AUDIO_IMPORTER_H
