#ifndef FMOD_AUDIO_PREVIEW_INSPECTOR_H
#define FMOD_AUDIO_PREVIEW_INSPECTOR_H

#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/classes/editor_property.hpp>
#include "audio/fmod_audio_stream.h"

namespace godot {
	class FmodAudioPreviewInspector : public EditorInspectorPlugin {
		GDCLASS(FmodAudioPreviewInspector, EditorInspectorPlugin)

	protected:
		static void _bind_methods();

	public:
		virtual bool _can_handle(Object* object) const override;
		virtual void _parse_begin(Object* object) override;
	};
}

#endif // !FMOD_AUDIO_PREVIEW_INSPECTOR_H
