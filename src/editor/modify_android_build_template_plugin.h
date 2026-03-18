#ifndef MODIFY_ANDROID_BUILD_TEMPLATE_PLUGIN_H
#define MODIFY_ANDROID_BUILD_TEMPLATE_PLUGIN_H

#include <godot_cpp/classes/editor_plugin.hpp>

namespace godot {
	class ConfirmationDialog;
	class LineEdit;
	class FileDialog;
	class Button;

	class ModifyAndroidBuildTemplatePlugin : public EditorPlugin {
		GDCLASS(ModifyAndroidBuildTemplatePlugin, EditorPlugin)

	private:
		String template_path = "res://android/build";

		ConfirmationDialog* select_fmod_runtime_file_dialog = nullptr;
		LineEdit* file_path = nullptr;
		FileDialog* file_dialog = nullptr;
		Button* install_button = nullptr;

		void _modify_android_build_template_step1();
		void _modify_android_build_template_step2();
		void _create_dialog_to_editor(const String& title, const String& text);
		void _create_select_fmod_runtime_file_dialog();
		void _on_select_fmod_runtime_file_dialog_cancel_button_pressed();
		void _on_select_fmod_runtime_file_dialog_confirmed();
		void _on_file_button_pressed();
		void _on_file_dialog_file_selected(const String& p_path);
		void _on_install_button_pressed();

		// 辅助函数：复制文件
		Error _copy_file(const String& p_from, const String& p_to);
		// 辅助函数：确保目录存在
		Error _ensure_dir_exists(const String& p_path);

	protected:
		static void _bind_methods();
		void _notification(int p_what);

	public:

	};
}

#endif // !MODIFY_ANDROID_BUILD_TEMPLATE_PLUGIN_H
