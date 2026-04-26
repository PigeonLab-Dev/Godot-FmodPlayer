#ifndef FMOD_AUDIO_EFFECT_CHORUS_H
#define FMOD_AUDIO_EFFECT_CHORUS_H

#include "dsp/fmod_audio_effect.h"
#include <memory>

namespace godot {
	struct FmodAudioEffectChorusSharedState;

	class FmodAudioEffectChorus : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectChorus, FmodAudioEffect)

	public:
		static constexpr int32_t MAX_DELAY_MS = 50;
		static constexpr int32_t MAX_DEPTH_MS = 20;
		static constexpr int32_t MAX_WIDTH_MS = 50;
		static constexpr int32_t MAX_VOICES = 4;
		static constexpr int32_t CYCLES_FRAC = 16;
		static constexpr int32_t CYCLES_MASK = (1 << CYCLES_FRAC) - 1;
		static constexpr int32_t MAX_CHANNELS = 4;
		static constexpr int32_t MS_CUTOFF_MAX = 16000;

	protected:
		static void _bind_methods();
		void _validate_property(PropertyInfo& p_property) const;

	private:
		struct Voice {
			float delay = 12.0f;
			float rate = 1.0f;
			float depth = 0.0f;
			float level = 0.0f;
			float cutoff = static_cast<float>(MS_CUTOFF_MAX);
			float pan = 0.0f;
		} voice[MAX_VOICES];

		int voice_count = 2;
		float wet = 0.5f;
		float dry = 1.0f;

		std::shared_ptr<FmodAudioEffectChorusSharedState> chorus_state;

		void _sync_shared_state();

	public:
		FmodAudioEffectChorus();
		virtual ~FmodAudioEffectChorus();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;
		virtual Ref<FmodDSP> create_custom_dsp(Ref<FmodSystem> system) override;
		virtual FMOD_DSP_DESCRIPTION* get_dsp_description() override;

		void set_voice_count(int p_voices);
		int get_voice_count() const;

		void set_voice_delay_ms(int p_voice, float p_delay_ms);
		float get_voice_delay_ms(int p_voice) const;

		void set_voice_rate_hz(int p_voice, float p_rate_hz);
		float get_voice_rate_hz(int p_voice) const;

		void set_voice_depth_ms(int p_voice, float p_depth_ms);
		float get_voice_depth_ms(int p_voice) const;

		void set_voice_level_db(int p_voice, float p_level_db);
		float get_voice_level_db(int p_voice) const;

		void set_voice_cutoff_hz(int p_voice, float p_cutoff_hz);
		float get_voice_cutoff_hz(int p_voice) const;

		void set_voice_pan(int p_voice, float p_pan);
		float get_voice_pan(int p_voice) const;

		void set_wet(float amount);
		float get_wet() const;

		void set_dry(float amount);
		float get_dry() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_CHORUS_H
