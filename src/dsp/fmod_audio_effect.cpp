#include "dsp/fmod_audio_effect.h"
#include "core/fmod_system.h"
#include "core/fmod_server.h"
#include <string.h>

namespace godot {
	void FmodAudioEffect::_bind_methods() {
		ClassDB::bind_method(D_METHOD("apply_to", "bus"), &FmodAudioEffect::apply_to);
		ClassDB::bind_method(D_METHOD("remove_from_bus", "bus"), &FmodAudioEffect::remove_from_bus);
		ClassDB::bind_method(D_METHOD("get_bus"), &FmodAudioEffect::get_bus);
	}

	FmodAudioEffect::FmodAudioEffect() {
	}

	FmodAudioEffect::~FmodAudioEffect() {
		if (bus.is_valid()) {
			for (int i = 0; i < dsp_chain.size(); i++) {
				if (dsp_chain[i].is_valid()) {
					bus->remove_dsp(dsp_chain[i]);
				}
			}
			dsp_chain.clear();
		}
	}

	void FmodAudioEffect::apply_to(Ref<FmodChannelGroup> p_bus) {
		// 基类默认实现 - 子类应该重写以添加自己的 DSP
	}

	void FmodAudioEffect::remove_from_bus(Ref<FmodChannelGroup> p_bus) {
		if (p_bus.is_valid()) {
			for (int i = 0; i < dsp_chain.size(); i++) {
				if (dsp_chain[i].is_valid()) {
					p_bus->remove_dsp(dsp_chain[i]);
				}
			}
		}
		dsp_chain.clear();
		bus.unref();
	}

	Ref<FmodChannelGroup> FmodAudioEffect::get_bus() const {
		return bus;
	}

	Vector<Ref<FmodDSP>> FmodAudioEffect::get_dsp_chain() const {
		return dsp_chain;
	}
	
	// 虚函数默认实现
	void FmodAudioEffect::_on_dsp_create(FMOD_DSP_STATE* dsp_state) {
		// 基类默认实现 - 分配自定义状态
		CustomDSPState* state = new CustomDSPState();
		state->effect = this;
		dsp_state->instance = state;
	}
	
	void FmodAudioEffect::_on_dsp_process(FMOD_DSP_STATE* dsp_state,
		unsigned int length,
		const FMOD_DSP_BUFFER_ARRAY* inbufferarray,
		FMOD_DSP_BUFFER_ARRAY* outbufferarray,
		FMOD_BOOL inputsidle,
		FMOD_DSP_PROCESS_OPERATION op) {
		// 基类默认实现 - 直接旁路
		if (op == FMOD_DSP_PROCESS_QUERY) {
			// 设置输出格式与输入相同
			outbufferarray->buffernumchannels[0] = inbufferarray->buffernumchannels[0];
			outbufferarray->bufferchannelmask[0] = inbufferarray->bufferchannelmask[0];
			outbufferarray->speakermode = inbufferarray->speakermode;
		} else if (op == FMOD_DSP_PROCESS_PERFORM) {
			// 直接拷贝输入到输出
			int num_channels = inbufferarray->buffernumchannels[0];
			memcpy(outbufferarray->buffers[0], inbufferarray->buffers[0], 
				length * num_channels * sizeof(float));
		}
	}
	
	void FmodAudioEffect::_on_dsp_release(FMOD_DSP_STATE* dsp_state) {
		// 基类默认实现 - 释放自定义状态
		if (dsp_state->instance) {
			CustomDSPState* state = static_cast<CustomDSPState*>(dsp_state->instance);
			delete state;
			dsp_state->instance = nullptr;
		}
	}
	
	// 获取 DSP 描述 - 子类可以重写
	FMOD_DSP_DESCRIPTION* FmodAudioEffect::get_dsp_description() {
		static FMOD_DSP_DESCRIPTION desc = {};
		static bool initialized = false;
		
		if (!initialized) {
			memset(&desc, 0, sizeof(desc));
			desc.pluginsdkversion = FMOD_PLUGIN_SDK_VERSION;
			strncpy(desc.name, "CustomEffect", sizeof(desc.name) - 1);
			desc.version = 1;
			desc.numinputbuffers = 1;
			desc.numoutputbuffers = 1;
			desc.create = fmod_custom_dsp_create_callback;
			desc.release = fmod_custom_dsp_release_callback;
			desc.process = fmod_custom_dsp_process_callback;
			initialized = true;
		}
		return &desc;
	}
	
	// 创建自定义 DSP
	Ref<FmodDSP> FmodAudioEffect::create_custom_dsp(Ref<FmodSystem> system) {
		ERR_FAIL_COND_V(system.is_null(), Ref<FmodDSP>());
		
		FMOD::System* fmod_system = system->get_system();
		ERR_FAIL_COND_V(!fmod_system, Ref<FmodDSP>());
		
		// 为每个实例创建独立的 DSP 描述符副本
		// 注意：FMOD_DSP_DESCRIPTION 中的回调是静态的，但 userdata 需要指向当前实例
		FMOD_DSP_DESCRIPTION* base_desc = get_dsp_description();
		ERR_FAIL_COND_V(!base_desc, Ref<FmodDSP>());
		
		// 分配新的描述符（FMOD 会复制这个结构，所以我们可以使用栈内存）
		FMOD_DSP_DESCRIPTION desc;
		memcpy(&desc, base_desc, sizeof(FMOD_DSP_DESCRIPTION));
		
		// 关键：设置 userdata 为当前效果器实例
		// 这样回调函数才能识别是哪个实例
		desc.userdata = this;
		
		FMOD::DSP* dsp_ptr = nullptr;
		FMOD_RESULT result = fmod_system->createDSP(&desc, &dsp_ptr);
		if (result != FMOD_OK || !dsp_ptr) {
			ERR_PRINT(vformat("Failed to create custom DSP: %s", FMOD_ErrorString(result)));
			return Ref<FmodDSP>();
		}
		
		Ref<FmodDSP> dsp;
		dsp.instantiate();
		dsp->setup(dsp_ptr);
		
		return dsp;
	}
}

// C 回调函数实现
extern "C" {
	FMOD_RESULT F_CALL fmod_custom_dsp_create_callback(FMOD_DSP_STATE* dsp_state) {
		// 安全检查
		if (!dsp_state) {
			return FMOD_ERR_INVALID_PARAM;
		}
		
		// 获取 userdata 中的效果对象指针
		void* userdata = nullptr;
		FMOD_DSP_STATE_FUNCTIONS* funcs = dsp_state->functions;
		if (funcs && funcs->getuserdata) {
			funcs->getuserdata(dsp_state, &userdata);
		}
		
		// 如果没有 userdata，分配一个空状态并返回
		if (!userdata) {
			// 分配一个空的自定义状态
			godot::CustomDSPState* state = new godot::CustomDSPState();
			dsp_state->instance = state;
			return FMOD_OK;
		}
		
		// 调用 C++ 虚函数
		godot::FmodAudioEffect* effect = static_cast<godot::FmodAudioEffect*>(userdata);
		if (effect) {
			effect->_on_dsp_create(dsp_state);
		}
		
		return FMOD_OK;
	}
	
	FMOD_RESULT F_CALL fmod_custom_dsp_process_callback(
		FMOD_DSP_STATE* dsp_state,
		unsigned int length,
		const FMOD_DSP_BUFFER_ARRAY* inbufferarray,
		FMOD_DSP_BUFFER_ARRAY* outbufferarray,
		FMOD_BOOL inputsidle,
		FMOD_DSP_PROCESS_OPERATION op
	) {
		// 安全检查
		if (!dsp_state || !inbufferarray || !outbufferarray) {
			return FMOD_ERR_INVALID_PARAM;
		}
		
		// 从 instance 获取自定义状态
		if (!dsp_state->instance) {
			// 没有自定义状态，直接旁路
			if (op == FMOD_DSP_PROCESS_QUERY) {
				outbufferarray->buffernumchannels[0] = inbufferarray->buffernumchannels[0];
				outbufferarray->bufferchannelmask[0] = inbufferarray->bufferchannelmask[0];
				outbufferarray->speakermode = inbufferarray->speakermode;
			} else if (op == FMOD_DSP_PROCESS_PERFORM) {
				int num_channels = inbufferarray->buffernumchannels[0];
				memcpy(outbufferarray->buffers[0], inbufferarray->buffers[0], 
					length * num_channels * sizeof(float));
			}
			return FMOD_OK;
		}
		
		// 获取效果对象并调用虚函数
		godot::CustomDSPState* state = static_cast<godot::CustomDSPState*>(dsp_state->instance);
		if (state->effect) {
			state->effect->_on_dsp_process(dsp_state, length, inbufferarray, outbufferarray, inputsidle, op);
		} else {
			// 没有效果对象，直接旁路
			if (op == FMOD_DSP_PROCESS_QUERY) {
				outbufferarray->buffernumchannels[0] = inbufferarray->buffernumchannels[0];
				outbufferarray->bufferchannelmask[0] = inbufferarray->bufferchannelmask[0];
				outbufferarray->speakermode = inbufferarray->speakermode;
			} else if (op == FMOD_DSP_PROCESS_PERFORM) {
				int num_channels = inbufferarray->buffernumchannels[0];
				memcpy(outbufferarray->buffers[0], inbufferarray->buffers[0], 
					length * num_channels * sizeof(float));
			}
		}
		
		return FMOD_OK;
	}
	
	FMOD_RESULT F_CALL fmod_custom_dsp_release_callback(FMOD_DSP_STATE* dsp_state) {
		if (!dsp_state) {
			return FMOD_ERR_INVALID_PARAM;
		}
		
		if (!dsp_state->instance) {
			return FMOD_OK;
		}
		
		// 获取效果对象并调用虚函数
		godot::CustomDSPState* state = static_cast<godot::CustomDSPState*>(dsp_state->instance);
		if (state->effect) {
			state->effect->_on_dsp_release(dsp_state);
		} else {
			// 没有效果对象，直接释放状态
			delete state;
			dsp_state->instance = nullptr;
		}
		
		return FMOD_OK;
	}
}
