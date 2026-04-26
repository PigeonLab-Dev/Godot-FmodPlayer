#ifndef FMOD_AUDIO_EFFECT_COMPRESSOR_H
#define FMOD_AUDIO_EFFECT_COMPRESSOR_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
	class FmodAudioEffectCompressor : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectCompressor, FmodAudioEffect)

	private:
		float threshold;
		float ratio;
		float gain;
		float attack_us;
		float release_ms;
		float mix;
		StringName sidechain;

		void _update_dsp_params();

	protected:
		static void _bind_methods();
		void _validate_property(PropertyInfo& p_property) const;

	public:
		FmodAudioEffectCompressor();
		virtual ~FmodAudioEffectCompressor();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		void set_threshold(float p_threshold);
		float get_threshold() const;

		void set_ratio(float p_ratio);
		float get_ratio() const;

		void set_gain(float p_gain);
		float get_gain() const;

		void set_attack_us(float p_attack_us);
		float get_attack_us() const;

		void set_release_ms(float p_release_ms);
		float get_release_ms() const;

		void set_mix(float p_mix);
		float get_mix() const;

		void set_sidechain(const StringName& p_sidechain);
		StringName get_sidechain() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_COMPRESSOR_H
