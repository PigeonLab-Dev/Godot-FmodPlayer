#include "modify_android_build_template_plugin.h"

#include <godot_cpp/classes/accept_dialog.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/grid_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <functional>

namespace godot {
	void ModifyAndroidBuildTemplatePlugin::_bind_methods() {

	}

	void ModifyAndroidBuildTemplatePlugin::_notification(int p_what) {
		switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			add_tool_menu_item("Install FMOD Android Libraries", callable_mp(this, &ModifyAndroidBuildTemplatePlugin::_modify_android_build_template_step1));
		} break;

		case NOTIFICATION_EXIT_TREE: {
			remove_tool_menu_item("Install FMOD Android Libraries");
		} break;
		}
	}

	void ModifyAndroidBuildTemplatePlugin::_modify_android_build_template_step1() {
		// 检查是否安装 Android 构建模板
		String global_template_path = ProjectSettings::get_singleton()->globalize_path(template_path);
		if (!DirAccess::dir_exists_absolute(global_template_path)) {
			_create_dialog_to_editor("Android Build Template Missing", 
				"Please install the Android build template first.\n\n"
				"In Godot editor: Project > Install Android Build Template...");
			return;
		}
		
		Ref<DirAccess> dir = DirAccess::open(template_path);
		ERR_FAIL_COND_MSG(dir.is_null(), "Failed to open \"" + template_path + "\"!");

		// 检查 build.gradle 文件是否存在
		if (!dir->file_exists("build.gradle")) {
			_create_dialog_to_editor("File Missing", 
				"Cannot find \"build.gradle\" file.\n"
				"Please ensure the Android build template is properly installed.");
			return;
		}

		// 显示选择 FMOD Android 运行库路径的对话框
		_create_select_fmod_runtime_file_dialog();
		select_fmod_runtime_file_dialog->popup_centered(Vector2(800, 150));
	}

	Error ModifyAndroidBuildTemplatePlugin::_ensure_dir_exists(const String& p_path) {
		Ref<DirAccess> dir = DirAccess::open("res://");
		ERR_FAIL_COND_V(dir.is_null(), FAILED);
		
		String path = p_path;
		if (path.begins_with("res://")) {
			path = path.substr(6);
		}
		
		PackedStringArray folders = path.split("/", false);
		String current = "";
		for (int i = 0; i < folders.size(); i++) {
			current += folders[i] + "/";
			if (!DirAccess::dir_exists_absolute("res://" + current)) {
				Error err = dir->make_dir(current);
				if (err != OK) {
					return err;
				}
			}
		}
		return OK;
	}

	Error ModifyAndroidBuildTemplatePlugin::_copy_file(const String& p_from, const String& p_to) {
		Ref<FileAccess> src = FileAccess::open(p_from, FileAccess::READ);
		ERR_FAIL_COND_V_MSG(src.is_null(), FAILED, "Cannot open source file: " + p_from);
		
		Ref<FileAccess> dst = FileAccess::open(p_to, FileAccess::WRITE);
		ERR_FAIL_COND_V_MSG(dst.is_null(), FAILED, "Cannot create destination file: " + p_to);
		
		PackedByteArray buffer = src->get_buffer(src->get_length());
		dst->store_buffer(buffer);
		dst->flush();
		
		return OK;
	}

	void ModifyAndroidBuildTemplatePlugin::_modify_android_build_template_step2() {
		ERR_FAIL_COND(!file_path);
		String path = file_path->get_text().strip_edges();
		
		if (path.is_empty()) {
			_create_dialog_to_editor("Path Empty", "Please select the FMOD Android API folder.");
			return;
		}

		// 转换为绝对路径并标准化
		String fmod_api_path = ProjectSettings::get_singleton()->globalize_path(path);
		
		// 检查路径是否存在且是目录
		if (!DirAccess::dir_exists_absolute(fmod_api_path)) {
			_create_dialog_to_editor("Invalid Path", 
				"The selected path does not exist or is not a directory.\n"
				"Please select the extracted FMOD Android API folder (e.g., fmodstudioapi20312android).");
			return;
		}

		// 尝试不同的可能路径结构
		String core_lib_path;
		String fmod_jar_path = fmod_api_path + "/api/core/lib/fmod.jar";
		
		if (FileAccess::file_exists(fmod_jar_path)) {
			core_lib_path = fmod_api_path + "/api/core/lib";
		} else {
			// 尝试直接在子目录中查找
			fmod_jar_path = fmod_api_path + "/fmodstudioapi*/api/core/lib/fmod.jar";
			// 简单检查：看看是否有一个子目录
			Ref<DirAccess> dir = DirAccess::open(path);
			if (dir.is_valid()) {
				dir->list_dir_begin();
				String subdir = dir->get_next();
				while (!subdir.is_empty()) {
					if (dir->current_is_dir() && subdir != "." && subdir != "..") {
						String test_path = fmod_api_path + "/" + subdir + "/api/core/lib/fmod.jar";
						if (FileAccess::file_exists(test_path)) {
							core_lib_path = fmod_api_path + "/" + subdir + "/api/core/lib";
							break;
						}
					}
					subdir = dir->get_next();
				}
			}
		}

		if (core_lib_path.is_empty()) {
			_create_dialog_to_editor("Invalid FMOD API Path", 
				"Cannot find \"api/core/lib/fmod.jar\" in the selected folder.\n"
				"Please ensure you selected the correct FMOD Android API folder.");
			return;
		}

		// 检查关键文件是否存在
		if (!FileAccess::file_exists(core_lib_path + "/fmod.jar")) {
			_create_dialog_to_editor("fmod.jar Missing", 
				"Cannot find \"fmod.jar\" in: " + core_lib_path);
			return;
		}

		// 架构映射表
		struct ArchInfo {
			String fmod_arch;
			String godot_arch;
		};
		ArchInfo arch_mappings[] = {
			{"arm64-v8a", "arm64-v8a"},
			{"armeabi-v7a", "armeabi-v7a"},
			{"x86", "x86"},
			{"x86_64", "x86_64"}
		};

		// 检查至少有一个架构的库文件存在
		bool has_any_lib = false;
		for (int i = 0; i < 4; i++) {
			if (FileAccess::file_exists(core_lib_path + "/" + arch_mappings[i].fmod_arch + "/libfmod.so")) {
				has_any_lib = true;
				break;
			}
		}
		if (!has_any_lib) {
			_create_dialog_to_editor("Native Libraries Missing", 
				"Cannot find any native libraries (libfmod.so) in the expected architecture folders.\n"
				"Expected folders: arm64-v8a, armeabi-v7a, x86, x86_64");
			return;
		}

		// 开始安装
		Error err;
		int success_count = 0;
		int total_count = 0;
		String error_messages;

		// 创建 libs 目录结构并复制 fmod.jar
		String libs_path = template_path + "/libs";
		err = _ensure_dir_exists(libs_path);
		if (err != OK) {
			_create_dialog_to_editor("Directory Error", 
				"Failed to create libs directory. Error code: " + String::num_int64(err));
			return;
		}

		// 复制 fmod.jar
		err = _copy_file(core_lib_path + "/fmod.jar", libs_path + "/fmod.jar");
		total_count++;
		if (err == OK) {
			success_count++;
		} else {
			error_messages += "- Failed to copy fmod.jar\n";
		}

		// 创建架构目录并复制各架构的库文件
		for (int i = 0; i < 4; i++) {
			String arch = arch_mappings[i].godot_arch;
			String fmod_arch = arch_mappings[i].fmod_arch;
			
			// Debug 目录
			String debug_arch_path = libs_path + "/debug/" + arch;
			err = _ensure_dir_exists(debug_arch_path);
			if (err == OK) {
				// 复制 debug 版本 (libfmodL.so)
				String debug_src = core_lib_path + "/" + fmod_arch + "/libfmodL.so";
				if (FileAccess::file_exists(debug_src)) {
					err = _copy_file(debug_src, debug_arch_path + "/libfmod.so");
					total_count++;
					if (err == OK) {
						success_count++;
					} else {
						error_messages += "- Failed to copy " + arch + " debug library\n";
					}
				}
			}
			
			// Release 目录
			String release_arch_path = libs_path + "/release/" + arch;
			err = _ensure_dir_exists(release_arch_path);
			if (err == OK) {
				// 复制 release 版本 (libfmod.so)
				String release_src = core_lib_path + "/" + fmod_arch + "/libfmod.so";
				if (FileAccess::file_exists(release_src)) {
					err = _copy_file(release_src, release_arch_path + "/libfmod.so");
					total_count++;
					if (err == OK) {
						success_count++;
					} else {
						error_messages += "- Failed to copy " + arch + " release library\n";
					}
				}
			}
		}

		// 关闭对话框
		if (select_fmod_runtime_file_dialog) {
			select_fmod_runtime_file_dialog->queue_free();
			select_fmod_runtime_file_dialog = nullptr;
			file_path = nullptr;
			file_dialog = nullptr;
			install_button = nullptr;
		}

		// 显示结果
		if (success_count == total_count) {
			_create_dialog_to_editor("Installation Successful", 
				"FMOD Android libraries have been installed successfully!\n\n"
				"Installed files:\n"
				"- fmod.jar\n"
				"- Native libraries for: arm64-v8a, armeabi-v7a, x86, x86_64\n"
				"  (both debug and release versions)\n\n"
				"You can now export your project for Android.");
		} else {
			_create_dialog_to_editor("Installation Partially Failed", 
				"Some files could not be copied.\n\n"
				"Success: " + String::num_int64(success_count) + "/" + String::num_int64(total_count) + "\n\n"
				"Errors:\n" + error_messages);
		}
	}

	void ModifyAndroidBuildTemplatePlugin::_create_select_fmod_runtime_file_dialog() {
		select_fmod_runtime_file_dialog = memnew(ConfirmationDialog);
		select_fmod_runtime_file_dialog->set_title("Install FMOD Android Libraries");
		select_fmod_runtime_file_dialog->set_min_size(Vector2(800, 200));
		
		// 主容器
		VBoxContainer* main_vbox = memnew(VBoxContainer);
		main_vbox->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		select_fmod_runtime_file_dialog->add_child(main_vbox);

		// 说明标签
		Label* info_label = memnew(Label);
		info_label->set_text("Select the extracted FMOD Android API folder:\n"
			"(e.g., C:\\Users\\Username\\Downloads\\fmodstudioapi20312android)");
		main_vbox->add_child(info_label);

		// 路径选择行
		HBoxContainer* hb = memnew(HBoxContainer);
		hb->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		main_vbox->add_child(hb);

		file_path = memnew(LineEdit);
		file_path->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		file_path->set_placeholder("Path to FMOD Android API folder...");
		hb->add_child(file_path);

		Button* file_button = memnew(Button);
		file_button->set_text("Browse...");
		file_button->connect("pressed", callable_mp(this, &ModifyAndroidBuildTemplatePlugin::_on_file_button_pressed));
		hb->add_child(file_button);

		// 添加间距
		main_vbox->add_child(memnew(Control));

		// 安装按钮
		install_button = memnew(Button);
		install_button->set_text("Install Libraries");
		install_button->set_custom_minimum_size(Vector2(150, 35));
		install_button->connect("pressed", callable_mp(this, &ModifyAndroidBuildTemplatePlugin::_on_install_button_pressed));
		main_vbox->add_child(install_button);

		// 文件浏览对话框
		file_dialog = memnew(FileDialog);
		file_dialog->set_file_mode(FileDialog::FILE_MODE_OPEN_DIR);
		file_dialog->set_access(FileDialog::ACCESS_FILESYSTEM);
		file_dialog->set_title("Select FMOD Android API Folder");
		file_dialog->connect("dir_selected", callable_mp(this, &ModifyAndroidBuildTemplatePlugin::_on_file_dialog_file_selected));
		select_fmod_runtime_file_dialog->add_child(file_dialog);

		// 连接对话框按钮
		select_fmod_runtime_file_dialog->get_cancel_button()->connect("pressed", 
			callable_mp(this, &ModifyAndroidBuildTemplatePlugin::_on_select_fmod_runtime_file_dialog_cancel_button_pressed));
		select_fmod_runtime_file_dialog->connect("confirmed", 
			callable_mp(this, &ModifyAndroidBuildTemplatePlugin::_on_select_fmod_runtime_file_dialog_confirmed));

		// 添加到编辑器
		Control* base_control = EditorInterface::get_singleton()->get_base_control();
		base_control->add_child(select_fmod_runtime_file_dialog);
	}

	void ModifyAndroidBuildTemplatePlugin::_create_dialog_to_editor(const String& title, const String& text) {
		AcceptDialog* dialog = memnew(AcceptDialog);
		dialog->set_title(title);
		dialog->set_text(text);
		Control* base_control = EditorInterface::get_singleton()->get_base_control();
		base_control->add_child(dialog);
		dialog->popup_centered();
	}

	void ModifyAndroidBuildTemplatePlugin::_on_select_fmod_runtime_file_dialog_cancel_button_pressed() {
		if (select_fmod_runtime_file_dialog) {
			select_fmod_runtime_file_dialog->queue_free();
			select_fmod_runtime_file_dialog = nullptr;
			file_path = nullptr;
			file_dialog = nullptr;
			install_button = nullptr;
		}
	}

	void ModifyAndroidBuildTemplatePlugin::_on_select_fmod_runtime_file_dialog_confirmed() {
		_modify_android_build_template_step2();
	}

	void ModifyAndroidBuildTemplatePlugin::_on_file_button_pressed() {
		ERR_FAIL_COND(!file_dialog);
		file_dialog->popup_centered(Vector2(800, 600));
	}

	void ModifyAndroidBuildTemplatePlugin::_on_file_dialog_file_selected(const String& p_path) {
		ERR_FAIL_COND(!file_path);
		file_path->set_text(p_path);
	}

	void ModifyAndroidBuildTemplatePlugin::_on_install_button_pressed() {
		_modify_android_build_template_step2();
	}
}
