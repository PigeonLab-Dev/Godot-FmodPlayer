#ifndef FMOD_AUDIO_EFFECT_CHORUS_H
#define FMOD_AUDIO_EFFECT_CHORUS_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
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
		static constexpr int32_t MS_CUTOFF_MAX = 22000;					// 匹配 FMOD DSP_LOWPASS 的最大值

	protected:
		static void _bind_methods();
		void _validate_property(PropertyInfo& p_property) const;

	private:
		struct Voice {
			float delay;
			float rate;
			float depth;
			float level;
			float cutoff;
			float pan;

			Voice() {
				delay = 12.0;
				rate = 1;
				depth = 0;
				level = 0;
				cutoff = MS_CUTOFF_MAX;
				pan = 0;
			}

		} voice[MAX_VOICES];

		int voice_count = 2;

		float wet = 0.5f;
		float dry = 1.0f;

	private:
		void _create_merged_chorus_dsp(Ref<FmodSystem> system);

	public:
		FmodAudioEffectChorus();
		virtual ~FmodAudioEffectChorus();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

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

