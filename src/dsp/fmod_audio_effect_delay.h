#ifndef FMOD_AUDIO_EFFECT_DELAY_H
#define FMOD_AUDIO_EFFECT_DELAY_H

#include "dsp/fmod_audio_effect.h"
#include "core/fmod_system.h"
#include <memory>

namespace godot {
	struct FmodAudioEffectDelaySharedState;

	class FmodAudioEffectDelay : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectDelay, FmodAudioEffect)

	private:
		float dry = 1.0f;

		bool tap_1_active = true;
		float tap_1_delay_ms = 250.0f;
		float tap_1_level = -6.0f;
		float tap_1_pan = 0.2f;

		bool tap_2_active = true;
		float tap_2_delay_ms = 500.0f;
		float tap_2_level = -12.0f;
		float tap_2_pan = -0.4f;

		bool feedback_active = false;
		float feedback_delay_ms = 340.0f;
		float feedback_level = -6.0f;
		float feedback_lowpass = 16000.0f;

		std::shared_ptr<FmodAudioEffectDelaySharedState> delay_state;

		void _sync_shared_state();

	protected:
		static void _bind_methods();

	public:
		FmodAudioEffectDelay();
		virtual ~FmodAudioEffectDelay();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;
		virtual Ref<FmodDSP> create_custom_dsp(Ref<FmodSystem> system) override;
		virtual FMOD_DSP_DESCRIPTION* get_dsp_description() override;

		void set_dry(float p_dry);
		float get_dry();

		void set_tap1_active(bool p_active);
		bool is_tap1_active() const;

		void set_tap1_delay_ms(float p_delay_ms);
		float get_tap1_delay_ms() const;

		void set_tap1_level_db(float p_level_db);
		float get_tap1_level_db() const;

		void set_tap1_pan(float p_pan);
		float get_tap1_pan() const;

		void set_tap2_active(bool p_active);
		bool is_tap2_active() const;

		void set_tap2_delay_ms(float p_delay_ms);
		float get_tap2_delay_ms() const;

		void set_tap2_level_db(float p_level_db);
		float get_tap2_level_db() const;

		void set_tap2_pan(float p_pan);
		float get_tap2_pan() const;

		void set_feedback_active(bool p_active);
		bool is_feedback_active() const;

		void set_feedback_delay_ms(float p_delay_ms);
		float get_feedback_delay_ms() const;

		void set_feedback_level_db(float p_level_db);
		float get_feedback_level_db() const;

		void set_feedback_lowpass(float p_lowpass);
		float get_feedback_lowpass() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_DELAY_H
