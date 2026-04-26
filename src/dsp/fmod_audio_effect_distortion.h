#ifndef FMOD_AUDIO_EFFECT_DISTORTION_H
#define FMOD_AUDIO_EFFECT_DISTORTION_H

#include "dsp/fmod_audio_effect.h"
#include <memory>

namespace godot {
	struct FmodAudioEffectDistortionSharedState;

	class FmodAudioEffectDistortion : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectDistortion, FmodAudioEffect)

	public:
		enum Mode {
			MODE_CLIP = 0,
			MODE_ATAN = 1,
			MODE_LOFI = 2,
			MODE_OVERDRIVE = 3,
			MODE_WAVESHAPE = 4,
			MODE_BITCRUSH = 5
		};

	protected:
		static void _bind_methods();

	private:
		Mode mode = MODE_CLIP;
		float pre_gain = 0.0f;
		float drive = 0.0f;
		float post_gain = 0.0f;
		float keep_hf_hz = 16000.0f;
		int oversample = 4;

		std::shared_ptr<FmodAudioEffectDistortionSharedState> distortion_state;

		void _sync_shared_state();

	public:
		FmodAudioEffectDistortion();
		virtual ~FmodAudioEffectDistortion();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;
		virtual Ref<FmodDSP> create_custom_dsp(Ref<FmodSystem> system) override;
		virtual FMOD_DSP_DESCRIPTION* get_dsp_description() override;

		void set_mode(Mode p_mode);
		Mode get_mode() const;

		void set_pre_gain(float p_gain);
		float get_pre_gain() const;

		void set_drive(float p_drive);
		float get_drive() const;

		void set_post_gain(float p_gain);
		float get_post_gain() const;

		void set_keep_hf_hz(float p_hz);
		float get_keep_hf_hz() const;

		void set_oversample(int p_oversample);
		int get_oversample() const;
	};
}

VARIANT_ENUM_CAST(FmodAudioEffectDistortion::Mode);

#endif // !FMOD_AUDIO_EFFECT_DISTORTION_H
