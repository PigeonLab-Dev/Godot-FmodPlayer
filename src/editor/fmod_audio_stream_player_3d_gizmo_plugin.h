#ifndef FMOD_AUDIO_STREAM_PLAYER_3D_GIZMO_PLUGIN_H
#define FMOD_AUDIO_STREAM_PLAYER_3D_GIZMO_PLUGIN_H

#include "core/fmod_utils.hpp"
#include <godot_cpp/classes/editor_node3d_gizmo_plugin.hpp>
#include <godot_cpp/classes/texture2d.hpp>

namespace godot {
	class FmodAudioStreamPlayer3DGizmoPlugin : public EditorNode3DGizmoPlugin {
		GDCLASS(FmodAudioStreamPlayer3DGizmoPlugin, EditorNode3DGizmoPlugin)

	protected:
		static void _bind_methods() {}

	public:
		FmodAudioStreamPlayer3DGizmoPlugin();

		virtual bool _has_gizmo(Node3D* p_for_node_3d) const override;
		virtual String _get_gizmo_name() const override;
		virtual int32_t _get_priority() const override;

		virtual void _redraw(const Ref<EditorNode3DGizmo>& p_gizmo) override;
	};
}

#endif // !FMOD_AUDIO_STREAM_PLAYER_3D_GIZMO_PLUGIN_H