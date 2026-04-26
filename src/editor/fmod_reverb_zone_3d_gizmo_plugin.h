#ifndef FMOD_REVERB_ZONE_3D_GIZMO_PLUGIN_H
#define FMOD_REVERB_ZONE_3D_GIZMO_PLUGIN_H

#include <godot_cpp/classes/editor_node3d_gizmo_plugin.hpp>

namespace godot {
	class FmodReverbZone3DGizmoPlugin : public EditorNode3DGizmoPlugin {
		GDCLASS(FmodReverbZone3DGizmoPlugin, EditorNode3DGizmoPlugin)

	private:
		PackedVector3Array _make_sphere_lines(float p_radius) const;

	protected:
		static void _bind_methods() {}

	public:
		FmodReverbZone3DGizmoPlugin();

		virtual bool _has_gizmo(Node3D* p_for_node_3d) const override;
		virtual String _get_gizmo_name() const override;
		virtual int32_t _get_priority() const override;
		virtual void _redraw(const Ref<EditorNode3DGizmo>& p_gizmo) override;
	};
}

#endif // FMOD_REVERB_ZONE_3D_GIZMO_PLUGIN_H
