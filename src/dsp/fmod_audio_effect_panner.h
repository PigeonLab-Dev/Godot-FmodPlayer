#ifndef FMOD_AUDIO_EFFECT_PANNER_H
#define FMOD_AUDIO_EFFECT_PANNER_H

#include "dsp/fmod_audio_effect.h"
#include <memory>

namespace godot {
	struct FmodAudioEffectPannerSharedState;

	class FmodAudioEffectPanner : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectPanner, FmodAudioEffect)

	private:
		float pan = 0.0f;
		std::shared_ptr<FmodAudioEffectPannerSharedState> panner_state;

		void _sync_shared_state();

	protected:
		static void _bind_methods();

	public:
		FmodAudioEffectPanner();
		virtual ~FmodAudioEffectPanner();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;
		virtual Ref<FmodDSP> create_custom_dsp(Ref<FmodSystem> system) override;
		virtual FMOD_DSP_DESCRIPTION* get_dsp_description() override;

		void set_pan(float p_pan);
		float get_pan() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_PANNER_H
