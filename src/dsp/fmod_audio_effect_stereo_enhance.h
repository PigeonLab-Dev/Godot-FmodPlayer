#ifndef FMOD_AUDIO_EFFECT_STEREO_ENHANCE_H
#define FMOD_AUDIO_EFFECT_STEREO_ENHANCE_H

#include "dsp/fmod_audio_effect.h"
#include <memory>

namespace godot {
	struct FmodAudioEffectStereoEnhanceSharedState;

	class FmodAudioEffectStereoEnhance : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectStereoEnhance, FmodAudioEffect)

	protected:
		static void _bind_methods();

	private:
		float pan_pullout = 1.0f;
		float time_pullout = 0.0f;
		float surround = 0.0f;
		std::shared_ptr<FmodAudioEffectStereoEnhanceSharedState> stereo_state;

		void _sync_shared_state();

	public:
		FmodAudioEffectStereoEnhance();
		virtual ~FmodAudioEffectStereoEnhance();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;
		virtual Ref<FmodDSP> create_custom_dsp(Ref<FmodSystem> system) override;
		virtual FMOD_DSP_DESCRIPTION* get_dsp_description() override;

		void set_pan_pullout(float p_amount);
		float get_pan_pullout() const;

		void set_time_pullout(float p_amount);
		float get_time_pullout() const;

		void set_surround(float p_amount);
		float get_surround() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_STEREO_ENHANCE_H
