#ifndef FMOD_AUDIO_EFFECT_RECORD_H
#define FMOD_AUDIO_EFFECT_RECORD_H

#include "dsp/fmod_audio_effect.h"
#include <godot_cpp/classes/audio_stream_wav.hpp>
#include <godot_cpp/templates/vector.hpp>

namespace godot {
	class FmodAudioEffectRecord : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectRecord, FmodAudioEffect)

	public:
		enum Format {
			FORMAT_8_BITS = 0,
			FORMAT_16_BITS = 1,
			FORMAT_IMA_ADPCM = 2
		};

	private:
		Format format = FORMAT_16_BITS;
		bool recording_active = false;
		
		// 录制缓冲区
		Vector<float> recording_buffer;
		int buffer_size = 0;
		int max_buffer_size = 0;
		
		// 用于自定义 DSP 的数据
		struct RecordDSPState {
			FmodAudioEffectRecord* effect;
		};
		
		void _init_recording();
		void _finish_recording();
		Ref<AudioStreamWAV> _create_wav_from_buffer() const;

	protected:
		static void _bind_methods();

	public:
		FmodAudioEffectRecord();
		virtual ~FmodAudioEffectRecord();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		virtual Ref<FmodDSP> create_custom_dsp(Ref<FmodSystem> system) override;
		virtual void _on_dsp_process(FMOD_DSP_STATE* dsp_state,
			unsigned int length,
			const FMOD_DSP_BUFFER_ARRAY* inbufferarray,
			FMOD_DSP_BUFFER_ARRAY* outbufferarray,
			FMOD_BOOL inputsidle,
			FMOD_DSP_PROCESS_OPERATION op) override;

		void set_recording_active(bool p_record);
		bool is_recording_active() const;

		void set_format(Format p_format);
		Format get_format() const;

		Ref<AudioStreamWAV> get_recording() const;
		
		// 添加样本到缓冲区（从 DSP 回调调用）
		void _add_samples(const float* buffer, unsigned int length, int channels);
	};
}

VARIANT_ENUM_CAST(FmodAudioEffectRecord::Format);

#endif
