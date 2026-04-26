#ifndef FMOD_GEOMETRY_MESH_TOOLS_PLUGIN_H
#define FMOD_GEOMETRY_MESH_TOOLS_PLUGIN_H

#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/classes/mesh.hpp>

#include <vector>

namespace godot {
	class AcceptDialog;
	class MenuButton;
	class MeshInstance3D;
	class Shape3D;

	class FmodGeometryMeshToolsPlugin : public EditorPlugin {
		GDCLASS(FmodGeometryMeshToolsPlugin, EditorPlugin)

	private:
		enum MenuOption {
			MENU_CREATE_TRIMESH = 0,
			MENU_CREATE_CONVEX = 1,
			MENU_CREATE_SIMPLIFIED_CONVEX = 2,
			MENU_CREATE_BOUNDING_BOX = 3,
			MENU_CREATE_MESH_RESOURCE = 4,
		};

		MenuButton* menu_button = nullptr;
		AcceptDialog* error_dialog = nullptr;
		MeshInstance3D* edited_mesh = nullptr;

		void _create_toolbar_menu();
		void _free_toolbar_menu();
		void _show_error(const String& p_text);
		void _on_menu_id_pressed(int p_id);
		void _create_fmod_geometry_from_selection(MenuOption p_option);
		std::vector<MeshInstance3D*> _get_selected_mesh_instances() const;
		Ref<Shape3D> _create_shape_from_mesh(const Ref<Mesh>& p_mesh, MenuOption p_option, Transform3D& r_shape_offset) const;

	protected:
		static void _bind_methods();
		void _notification(int p_what);

	public:
		virtual String _get_plugin_name() const override;
		virtual bool _has_main_screen() const override;
		virtual bool _handles(Object* p_object) const override;
		virtual void _edit(Object* p_object) override;
		virtual void _make_visible(bool p_visible) override;
	};
}

#endif // FMOD_GEOMETRY_MESH_TOOLS_PLUGIN_H
