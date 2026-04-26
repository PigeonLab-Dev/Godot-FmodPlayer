#ifndef MODIFY_ANDROID_BUILD_TEMPLATE_PLUGIN_H
#define MODIFY_ANDROID_BUILD_TEMPLATE_PLUGIN_H

#include <godot_cpp/classes/editor_plugin.hpp>

namespace godot {
	class AcceptDialog;
	class Button;
	class ConfirmationDialog;
	class FileDialog;
	class LineEdit;
	class RichTextLabel;

	class ModifyAndroidBuildTemplatePlugin : public EditorPlugin {
		GDCLASS(ModifyAndroidBuildTemplatePlugin, EditorPlugin)

	private:
		struct FmodAndroidSdkInfo {
			String input_path;
			String extracted_root;
			String core_lib_path;
		};

		String template_path = "res://android/build";
		String selected_fmod_path;
		FmodAndroidSdkInfo pending_sdk;

		ConfirmationDialog* sdk_select_dialog = nullptr;
		ConfirmationDialog* review_dialog = nullptr;
		LineEdit* file_path = nullptr;
		FileDialog* file_dialog = nullptr;
		RichTextLabel* review_text = nullptr;

		void _open_configure_dialog();
		void _create_sdk_select_dialog();
		void _create_review_dialog(const String& p_summary);
		void _create_dialog_to_editor(const String& p_title, const String& p_text);
		void _free_sdk_select_dialog();
		void _free_review_dialog();

		void _on_browse_pressed();
		void _on_file_dialog_path_selected(const String& p_path);
		void _on_sdk_select_confirmed();
		void _on_sdk_select_canceled();
		void _on_review_confirmed();
		void _on_review_canceled();

		bool _validate_android_template(String& r_error) const;
		bool _resolve_fmod_sdk(const String& p_path, FmodAndroidSdkInfo& r_info, String& r_error);
		bool _extract_archive(const String& p_archive_path, String& r_extract_root, String& r_error) const;
		String _find_core_lib_path(const String& p_root, int p_depth = 0) const;
		bool _has_any_fmod_native_library(const String& p_core_lib_path) const;
		String _build_review_summary(const FmodAndroidSdkInfo& p_info) const;

		Error _apply_android_template_changes(const FmodAndroidSdkInfo& p_info, String& r_report);
		Error _copy_fmod_libraries(const String& p_core_lib_path, String& r_report);
		Error _copy_plugin_android_libraries(String& r_report);
		Error _patch_gradle_properties(String& r_report);
		Error _patch_build_gradle(String& r_report);
		Error _patch_godot_app_java(String& r_report);

		Error _copy_file(const String& p_from, const String& p_to);
		Error _ensure_dir_exists(const String& p_path) const;
		bool _write_text_file(const String& p_path, const String& p_text) const;
		String _read_text_file(const String& p_path) const;

	protected:
		static void _bind_methods();
		void _notification(int p_what);
	};
}

#endif // !MODIFY_ANDROID_BUILD_TEMPLATE_PLUGIN_H
