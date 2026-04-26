#ifndef FMOD_AUDIO_EFFECT_PHASER_H
#define FMOD_AUDIO_EFFECT_PHASER_H

#include "dsp/fmod_audio_effect.h"
#include <memory>

namespace godot {
	struct FmodAudioEffectPhaserSharedState;

	class FmodAudioEffectPhaser : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectPhaser, FmodAudioEffect)

	protected:
		static void _bind_methods();

	private:
		float depth = 1.0f;
		float feedback = 0.7f;
		float range_max_hz = 1600.0f;
		float range_min_hz = 440.0f;
		float rate_hz = 0.5f;
		std::shared_ptr<FmodAudioEffectPhaserSharedState> phaser_state;

		void _sync_shared_state();

	public:
		FmodAudioEffectPhaser();
		virtual ~FmodAudioEffectPhaser();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;
		virtual Ref<FmodDSP> create_custom_dsp(Ref<FmodSystem> system) override;
		virtual FMOD_DSP_DESCRIPTION* get_dsp_description() override;

		void set_depth(float p_depth);
		float get_depth() const;

		void set_feedback(float p_feedback);
		float get_feedback() const;

		void set_range_max_hz(float p_range_max_hz);
		float get_range_max_hz() const;

		void set_range_min_hz(float p_range_min_hz);
		float get_range_min_hz() const;

		void set_rate_hz(float p_rate_hz);
		float get_rate_hz() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_PHASER_H
