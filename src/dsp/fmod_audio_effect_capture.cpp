#include "fmod_audio_effect_capture.h"
#include "core/fmod_server.h"
#include <string.h>

namespace godot {
	void FmodAudioEffectCapture::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_buffer_length", "buffer_length"), &FmodAudioEffectCapture::set_buffer_length);
		ClassDB::bind_method(D_METHOD("get_buffer_length"), &FmodAudioEffectCapture::get_buffer_length);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "buffer_length", PROPERTY_HINT_RANGE, "0.1,10.0,0.1,suffix:s"), "set_buffer_length", "get_buffer_length");
		
		ClassDB::bind_method(D_METHOD("can_get_buffer", "frames"), &FmodAudioEffectCapture::can_get_buffer);
		ClassDB::bind_method(D_METHOD("clear_buffer"), &FmodAudioEffectCapture::clear_buffer);
		ClassDB::bind_method(D_METHOD("get_buffer", "frames"), &FmodAudioEffectCapture::get_buffer);
		ClassDB::bind_method(D_METHOD("get_discarded_frames"), &FmodAudioEffectCapture::get_discarded_frames);
		ClassDB::bind_method(D_METHOD("get_frames_available"), &FmodAudioEffectCapture::get_frames_available);
		ClassDB::bind_method(D_METHOD("get_pushed_frames"), &FmodAudioEffectCapture::get_pushed_frames);
	}

	FmodAudioEffectCapture::FmodAudioEffectCapture() {
		_update_buffer_size();
	}

	FmodAudioEffectCapture::~FmodAudioEffectCapture() {

	}
	
	void FmodAudioEffectCapture::_update_buffer_size() {
		// 假设默认采样率为 48000Hz，立体声
		// 实际采样率可以在 DSP 创建时获取
		int sample_rate = 48000;
		int channels = 2;
		int new_size = static_cast<int>(buffer_length * sample_rate * channels);
		
		// 确保缓冲区大小至少有一些空间
		if (new_size < 1024) {
			new_size = 1024;
		}
		
		// 限制最大缓冲区大小，防止内存溢出
		const int max_size = sample_rate * channels * 10; // 最大10秒
		if (new_size > max_size) {
			new_size = max_size;
		}
		
		buffer_size = new_size;
		
		std::lock_guard<std::mutex> lock(buffer_mutex);
		// 如果 DSP 正在运行，这里可能会有竞争条件
		// 但为了改变缓冲区大小，我们必须这样做
		ring_buffer.resize(static_cast<size_t>(buffer_size));
		ring_buffer.clear();
	}

	void FmodAudioEffectCapture::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");
		
		// 如果已经应用过，先移除
		if (bus.is_valid()) {
			remove_from_bus(bus);
		}
		
		bus = p_bus;
		
		// 获取主系统
		Ref<FmodSystem> system = FmodServer::get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");
		
		// 创建自定义 DSP
		Ref<FmodDSP> dsp = create_custom_dsp(system);
		if (!dsp.is_valid()) {
			ERR_PRINT("FmodAudioEffectCapture: Failed to create custom DSP");
			return;
		}
		
		// 检查 DSP 内部指针
		if (!dsp->dsp) {
			ERR_PRINT("FmodAudioEffectCapture: DSP internal pointer is null after setup");
			return;
		}
		
		// 添加到 DSP 链
		dsp_chain.push_back(dsp);
		
		// 添加到总线（-1 表示添加到末尾）
		bus->add_dsp(-1, dsp);
		
		dsp_created = true;
	}

	void FmodAudioEffectCapture::remove_from_bus(Ref<FmodChannelGroup> p_bus) {
		// 调用基类实现
		FmodAudioEffect::remove_from_bus(p_bus);
		
		// 清空缓冲区统计
		pushed_frames.store(0);
		discarded_frames.store(0);
		dsp_created = false;
	}
	
	// 重写基类虚函数 - DSP 创建
	void FmodAudioEffectCapture::_on_dsp_create(FMOD_DSP_STATE* dsp_state) {
		// 分配捕获特定的状态
		CaptureDSPState* state = new CaptureDSPState();
		state->ring_buffer = &ring_buffer;
		state->buffer_mutex = &buffer_mutex;
		state->pushed_frames = &pushed_frames;
		state->discarded_frames = &discarded_frames;
		state->channels = 2;														// 默认立体声
		
		// 存储到 DSP 状态
		dsp_state->instance = state;
	}
	
	// 重写基类虚函数 - DSP 处理
	void FmodAudioEffectCapture::_on_dsp_process(FMOD_DSP_STATE* dsp_state,
		unsigned int length,
		const FMOD_DSP_BUFFER_ARRAY* inbufferarray,
		FMOD_DSP_BUFFER_ARRAY* outbufferarray,
		FMOD_BOOL inputsidle,
		FMOD_DSP_PROCESS_OPERATION op) {
		
		// 安全检查：输入输出缓冲区数组
		if (!inbufferarray || !outbufferarray) {
			return;
		}
		
		if (op == FMOD_DSP_PROCESS_QUERY) {
			// 设置输出格式与输入相同
			outbufferarray->buffernumchannels[0] = inbufferarray->buffernumchannels[0];
			outbufferarray->bufferchannelmask[0] = inbufferarray->bufferchannelmask[0];
			outbufferarray->speakermode = inbufferarray->speakermode;
			return;
		}
		
		if (op == FMOD_DSP_PROCESS_PERFORM) {
			// 安全检查：输入输出缓冲区
			float* in = (float*)inbufferarray->buffers[0];
			float* out = (float*)outbufferarray->buffers[0];
			int ch = inbufferarray->buffernumchannels[0];
			
			if (!in || !out || ch <= 0) {
				return;
			}
			
			// 旁路输出 - 直接拷贝输入到输出（Capture 不改变音频）
			memcpy(out, in, length * ch * sizeof(float));
			
			// 写入环形缓冲区
			if (dsp_state->instance) {
				CaptureDSPState* state = static_cast<CaptureDSPState*>(dsp_state->instance);
				state->channels = ch;
				
				// 安全地访问环形缓冲区
				if (state->ring_buffer && state->buffer_mutex && state->pushed_frames && state->discarded_frames) {
					std::lock_guard<std::mutex> lock(*state->buffer_mutex);
					
					// 写入数据（交错格式）
					uint32_t samples_to_write = length * ch;
					size_t written = state->ring_buffer->write(in, static_cast<size_t>(samples_to_write));
					
					// 更新统计
					*state->pushed_frames += length;
					
					// 计算丢弃的采样
					if (written < samples_to_write) {
						uint32_t dropped_samples = samples_to_write - written;
						*state->discarded_frames += dropped_samples / ch;
					}
				}
			}
		}
	}
	
	// 重写基类虚函数 - DSP 释放
	void FmodAudioEffectCapture::_on_dsp_release(FMOD_DSP_STATE* dsp_state) {
		if (dsp_state->instance) {
			CaptureDSPState* state = static_cast<CaptureDSPState*>(dsp_state->instance);
			delete state;
			dsp_state->instance = nullptr;
		}
	}

	bool FmodAudioEffectCapture::can_get_buffer(const int64_t frames) const {
		std::lock_guard<std::mutex> lock(buffer_mutex);
		int64_t available_samples = ring_buffer.data_left();
		// 假设立体声（2声道）
		return (available_samples / 2) >= frames;
	}

	void FmodAudioEffectCapture::clear_buffer() {
		std::lock_guard<std::mutex> lock(buffer_mutex);
		ring_buffer.clear();
		pushed_frames.store(0);
		discarded_frames.store(0);
	}

	PackedVector2Array FmodAudioEffectCapture::get_buffer(const int64_t frames) {
		PackedVector2Array result;
		
		std::lock_guard<std::mutex> lock(buffer_mutex);
		
		size_t available_samples = ring_buffer.data_left();
		int64_t available_frames = static_cast<int64_t>(available_samples / 2);		// 假设立体声
		int64_t to_read = MIN(frames, available_frames);
		
		result.resize(static_cast<int>(to_read));
		
		for (int64_t i = 0; i < to_read; i++) {
			float l, r;
			ring_buffer.read(&l, 1);
			ring_buffer.read(&r, 1);
			result[i] = Vector2(l, r);
		}
		
		return result;
	}

	int64_t FmodAudioEffectCapture::get_discarded_frames() const {
		return discarded_frames.load();
	}

	int64_t FmodAudioEffectCapture::get_frames_available() const {
		std::lock_guard<std::mutex> lock(buffer_mutex);
		return static_cast<int64_t>(ring_buffer.data_left() / 2);					// 假设立体声
	}

	int64_t FmodAudioEffectCapture::get_pushed_frames() const {
		return pushed_frames.load();
	}

	void FmodAudioEffectCapture::set_buffer_length(double p_buffer_length) {
		ERR_FAIL_COND_MSG(p_buffer_length <= 0, "Buffer length must be positive");
		buffer_length = p_buffer_length;
		_update_buffer_size();
	}

	double FmodAudioEffectCapture::get_buffer_length() const {
		return buffer_length;
	}
}
