#ifndef FMOD_AUDIO_EFFECT_HARD_LIMITER_H
#define FMOD_AUDIO_EFFECT_HARD_LIMITER_H

#include "dsp/fmod_audio_effect.h"
#include <memory>

namespace godot {
	struct FmodAudioEffectHardLimiterSharedState;

	class FmodAudioEffectHardLimiter : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectHardLimiter, FmodAudioEffect)

	private:
		float ceiling_db = -0.3f;
		float pre_gain_db = 0.0f;
		float release = 0.1f;
		std::shared_ptr<FmodAudioEffectHardLimiterSharedState> limiter_state;

		void _sync_shared_state();

	protected:
		static void _bind_methods();

	public:
		FmodAudioEffectHardLimiter();
		virtual ~FmodAudioEffectHardLimiter();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;
		virtual Ref<FmodDSP> create_custom_dsp(Ref<FmodSystem> system) override;
		virtual FMOD_DSP_DESCRIPTION* get_dsp_description() override;

		void set_ceiling_db(float p_ceiling_db);
		float get_ceiling_db() const;

		void set_pre_gain_db(float p_pre_gain_db);
		float get_pre_gain_db() const;

		void set_release(float p_release);
		float get_release() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_HARD_LIMITER_H
