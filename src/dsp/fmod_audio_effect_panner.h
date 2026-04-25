#ifndef FMOD_AUDIO_EFFECT_PANNER_H
#define FMOD_AUDIO_EFFECT_PANNER_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
	class FmodAudioEffectPanner : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectPanner, FmodAudioEffect)

	private:
		float pan = 0.0f;

	protected:
		static void _bind_methods();

	public:
		FmodAudioEffectPanner();
		virtual ~FmodAudioEffectPanner();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		void set_pan(float p_pan);
		float get_pan() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_PANNER_H
