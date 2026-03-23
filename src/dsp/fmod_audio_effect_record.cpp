#include "dsp/fmod_audio_effect_record.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <cstring>

namespace godot {

	void FmodAudioEffectRecord::_bind_methods() {
		BIND_ENUM_CONSTANT(FORMAT_8_BITS);
		BIND_ENUM_CONSTANT(FORMAT_16_BITS);
		BIND_ENUM_CONSTANT(FORMAT_IMA_ADPCM);

		ClassDB::bind_method(D_METHOD("set_recording_active", "record"), &FmodAudioEffectRecord::set_recording_active);
		ClassDB::bind_method(D_METHOD("is_recording_active"), &FmodAudioEffectRecord::is_recording_active);
		ClassDB::bind_method(D_METHOD("set_format", "format"), &FmodAudioEffectRecord::set_format);
		ClassDB::bind_method(D_METHOD("get_format"), &FmodAudioEffectRecord::get_format);
		ClassDB::bind_method(D_METHOD("get_recording"), &FmodAudioEffectRecord::get_recording);

		ADD_PROPERTY(PropertyInfo(Variant::INT, "format", PROPERTY_HINT_ENUM, "8-bit,16-bit,IMA ADPCM"), "set_format", "get_format");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "recording_active"), "set_recording_active", "is_recording_active");
	}

	FmodAudioEffectRecord::FmodAudioEffectRecord() {
	}

	FmodAudioEffectRecord::~FmodAudioEffectRecord() {
		if (recording_active) {
			_finish_recording();
		}
	}

	void FmodAudioEffectRecord::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		if (bus == p_bus && !dsp_chain.is_empty()) {
			remove_from_bus(p_bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		// 创建自定义 DSP 用于录音
		Ref<FmodDSP> record_dsp = create_custom_dsp(system);
		if (!record_dsp.is_valid()) return;

		bus->add_dsp(-1, record_dsp);
		dsp_chain.push_back(record_dsp);

		if (recording_active) {
			_init_recording();
		}
	}

	// 自定义 DSP 回调，专门用于 Record
	static FMOD_RESULT F_CALL record_dsp_create_callback(FMOD_DSP_STATE* dsp_state) {
		// 获取 userdata
		void* userdata = nullptr;
		if (dsp_state->functions && dsp_state->functions->getuserdata) {
			dsp_state->functions->getuserdata(dsp_state, &userdata);
		}
		
		// 分配自定义状态
		godot::CustomDSPState* state = new godot::CustomDSPState();
		if (userdata) {
			state->effect = static_cast<godot::FmodAudioEffect*>(userdata);
		}
		dsp_state->instance = state;
		
		return FMOD_OK;
	}
	
	static FMOD_RESULT F_CALL record_dsp_process_callback(
		FMOD_DSP_STATE* dsp_state,
		unsigned int length,
		const FMOD_DSP_BUFFER_ARRAY* inbufferarray,
		FMOD_DSP_BUFFER_ARRAY* outbufferarray,
		FMOD_BOOL inputsidle,
		FMOD_DSP_PROCESS_OPERATION op
	) {
		if (op == FMOD_DSP_PROCESS_QUERY) {
			outbufferarray->buffernumchannels[0] = inbufferarray->buffernumchannels[0];
			outbufferarray->bufferchannelmask[0] = inbufferarray->bufferchannelmask[0];
			outbufferarray->speakermode = inbufferarray->speakermode;
			return FMOD_OK;
		}
		
		if (op != FMOD_DSP_PROCESS_PERFORM) {
			return FMOD_OK;
		}
		
		// 获取 effect 指针
		if (!dsp_state->instance) {
			// 没有状态，直接拷贝
			int num_channels = inbufferarray->buffernumchannels[0];
			memcpy(outbufferarray->buffers[0], inbufferarray->buffers[0], 
				length * num_channels * sizeof(float));
			return FMOD_OK;
		}
		
		godot::CustomDSPState* state = static_cast<godot::CustomDSPState*>(dsp_state->instance);
		if (state->effect) {
			state->effect->_on_dsp_process(dsp_state, length, inbufferarray, outbufferarray, inputsidle, op);
		} else {
			// 直接拷贝
			int num_channels = inbufferarray->buffernumchannels[0];
			memcpy(outbufferarray->buffers[0], inbufferarray->buffers[0], 
				length * num_channels * sizeof(float));
		}
		
		return FMOD_OK;
	}
	
	static FMOD_RESULT F_CALL record_dsp_release_callback(FMOD_DSP_STATE* dsp_state) {
		if (dsp_state->instance) {
			godot::CustomDSPState* state = static_cast<godot::CustomDSPState*>(dsp_state->instance);
			if (state->effect) {
				state->effect->_on_dsp_release(dsp_state);
			} else {
				delete state;
			}
			dsp_state->instance = nullptr;
		}
		return FMOD_OK;
	}

	Ref<FmodDSP> FmodAudioEffectRecord::create_custom_dsp(Ref<FmodSystem> system) {
		ERR_FAIL_COND_V(system.is_null(), Ref<FmodDSP>());

		FMOD::System* fmod_system = system->get_system();
		ERR_FAIL_COND_V(!fmod_system, Ref<FmodDSP>());

		static FMOD_DSP_DESCRIPTION desc = {};
		static bool initialized = false;

		if (!initialized) {
			memset(&desc, 0, sizeof(desc));
			desc.pluginsdkversion = FMOD_PLUGIN_SDK_VERSION;
			strncpy(desc.name, "RecordDSP", sizeof(desc.name) - 1);
			desc.version = 1;
			desc.numinputbuffers = 1;
			desc.numoutputbuffers = 1;
			desc.create = record_dsp_create_callback;
			desc.release = record_dsp_release_callback;
			desc.process = record_dsp_process_callback;
			initialized = true;
		}

		FMOD::DSP* dsp_ptr = nullptr;
		FMOD_RESULT result = fmod_system->createDSP(&desc, &dsp_ptr);
		if (result != FMOD_OK || !dsp_ptr) {
			UtilityFunctions::push_error("Failed to create custom Record DSP");
			return Ref<FmodDSP>();
		}

		Ref<FmodDSP> dsp;
		dsp.instantiate();
		// 直接设置 dsp 指针，不调用 setup，避免 setup 覆盖我们的回调
		dsp->dsp = dsp_ptr;
		// 设置 userdata 为 this（FmodAudioEffect*），回调需要这个来调用虚函数
		dsp_ptr->setUserData(this);

		return dsp;
	}

	void FmodAudioEffectRecord::_on_dsp_process(FMOD_DSP_STATE* dsp_state,
		unsigned int length,
		const FMOD_DSP_BUFFER_ARRAY* inbufferarray,
		FMOD_DSP_BUFFER_ARRAY* outbufferarray,
		FMOD_BOOL inputsidle,
		FMOD_DSP_PROCESS_OPERATION op) {

		if (op == FMOD_DSP_PROCESS_QUERY) {
			outbufferarray->buffernumchannels[0] = inbufferarray->buffernumchannels[0];
			outbufferarray->bufferchannelmask[0] = inbufferarray->bufferchannelmask[0];
			outbufferarray->speakermode = inbufferarray->speakermode;
			return;
		}

		if (op == FMOD_DSP_PROCESS_PERFORM) {
			int num_channels = inbufferarray->buffernumchannels[0];
			const float* in_buffer = inbufferarray->buffers[0];
			float* out_buffer = outbufferarray->buffers[0];

			// 拷贝输入到输出（旁路）
			memcpy(out_buffer, in_buffer, length * num_channels * sizeof(float));

			// 如果正在录制，添加样本到缓冲区
			if (recording_active) {
				_add_samples(in_buffer, length, num_channels);
			}
		}
	}

	void FmodAudioEffectRecord::_init_recording() {
		recording_buffer.clear();
		// 最大支持 5 分钟 48kHz 立体声
		max_buffer_size = 48000 * 60 * 5 * 2;
		buffer_size = 0;
	}

	void FmodAudioEffectRecord::_finish_recording() {
		// 缓冲区数据保留，直到下次开始录制或调用 get_recording
	}

	void FmodAudioEffectRecord::_add_samples(const float* buffer, unsigned int length, int channels) {
		if (!recording_active) return;

		// 将多声道混合为单声道或立体声
		if (channels == 1) {
			// 单声道直接存储
			for (unsigned int i = 0; i < length && buffer_size < max_buffer_size; i++) {
				recording_buffer.push_back(buffer[i]);
				buffer_size++;
			}
		} else if (channels >= 2) {
			// 立体声，存储左右声道
			for (unsigned int i = 0; i < length && buffer_size + 1 < max_buffer_size; i++) {
				// 左声道
				recording_buffer.push_back(buffer[i * channels]);
				// 右声道
				recording_buffer.push_back(buffer[i * channels + 1]);
				buffer_size += 2;
			}
		}
	}

	void FmodAudioEffectRecord::set_recording_active(bool p_record) {
		if (recording_active == p_record) {
			return;
		}

		if (p_record) {
			_init_recording();
		} else {
			_finish_recording();
		}
		recording_active = p_record;
	}

	bool FmodAudioEffectRecord::is_recording_active() const {
		return recording_active;
	}

	void FmodAudioEffectRecord::set_format(Format p_format) {
		format = p_format;
	}

	FmodAudioEffectRecord::Format FmodAudioEffectRecord::get_format() const {
		return format;
	}

	Ref<AudioStreamWAV> FmodAudioEffectRecord::get_recording() const {
		return _create_wav_from_buffer();
	}

	Ref<AudioStreamWAV> FmodAudioEffectRecord::_create_wav_from_buffer() const {
		if (recording_buffer.is_empty() || buffer_size == 0) {
			return Ref<AudioStreamWAV>();
		}

		Ref<AudioStreamWAV> wav;
		wav.instantiate();

		// 假设 48kHz 采样率
		int sample_rate = 48000;
		wav->set_mix_rate(sample_rate);
		wav->set_format(static_cast<AudioStreamWAV::Format>(format));
		wav->set_loop_mode(AudioStreamWAV::LOOP_DISABLED);
		wav->set_stereo(true);

		PackedByteArray data;

		switch (format) {
			case FORMAT_8_BITS: {
				data.resize(buffer_size);
				for (int i = 0; i < buffer_size; i++) {
					// float -1.0~1.0 转换为 8-bit 无符号 0~255，128 为中心点
					int32_t sample = static_cast<int32_t>(CLAMP(recording_buffer[i] * 127.0f + 128.0f, 0.0f, 255.0f));
					data[i] = static_cast<uint8_t>(sample);
				}
				break;
			}
			case FORMAT_16_BITS: {
				data.resize(buffer_size * 2);
				for (int i = 0; i < buffer_size; i++) {
					// float -1.0~1.0 转换为 16-bit
					int16_t sample = static_cast<int16_t>(CLAMP(recording_buffer[i] * 32767.0f, -32768.0f, 32767.0f));
					data[i * 2] = sample & 0xFF;
					data[i * 2 + 1] = (sample >> 8) & 0xFF;
				}
				break;
			}
			case FORMAT_IMA_ADPCM: {
				// IMA ADPCM 压缩需要专门的编码器，这里简化为 16-bit
				data.resize(buffer_size * 2);
				for (int i = 0; i < buffer_size; i++) {
					int16_t sample = static_cast<int16_t>(CLAMP(recording_buffer[i] * 32767.0f, -32768.0f, 32767.0f));
					data[i * 2] = sample & 0xFF;
					data[i * 2 + 1] = (sample >> 8) & 0xFF;
				}
				break;
			}
		}

		wav->set_data(data);
		return wav;
	}

}
