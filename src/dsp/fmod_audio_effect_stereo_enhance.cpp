#include "fmod_audio_effect_stereo_enhance.h"

namespace godot {
	void FmodAudioEffectStereoEnhance::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_pan_pullout", "amount"), &FmodAudioEffectStereoEnhance::set_pan_pullout);
		ClassDB::bind_method(D_METHOD("get_pan_pullout"), &FmodAudioEffectStereoEnhance::get_pan_pullout);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pan_pullout", PROPERTY_HINT_RANGE, "0,4,0.01"), "set_pan_pullout", "get_pan_pullout");

		ClassDB::bind_method(D_METHOD("set_time_pullout", "amount"), &FmodAudioEffectStereoEnhance::set_time_pullout);
		ClassDB::bind_method(D_METHOD("get_time_pullout"), &FmodAudioEffectStereoEnhance::get_time_pullout);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_pullout_ms", PROPERTY_HINT_RANGE, "0,50,0.01,suffix:ms"), "set_time_pullout", "get_time_pullout");

		ClassDB::bind_method(D_METHOD("set_surround", "amount"), &FmodAudioEffectStereoEnhance::set_surround);
		ClassDB::bind_method(D_METHOD("get_surround"), &FmodAudioEffectStereoEnhance::get_surround);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "surround", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_surround", "get_surround");
	}

	FmodAudioEffectStereoEnhance::FmodAudioEffectStereoEnhance() {

	}

	FmodAudioEffectStereoEnhance::~FmodAudioEffectStereoEnhance() {

	}

	void FmodAudioEffectStereoEnhance::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		if (bus == p_bus && !dsp_chain.is_empty()) {
			remove_from_bus(p_bus);
		}

		bus = p_bus;
		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		// 清空当前链，准备重新构建
		dsp_chain.clear();

		// 1. 如果需要时间拉宽，先创建 DELAY DSP（先延迟后声像）
		if (time_pullout > 0.0f) {
			Ref<FmodDSP> delay_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_DELAY);
			if (delay_dsp.is_valid()) {
				// 设置左右声道延迟：左 0 ms，右 time_pullout ms
				delay_dsp->set_parameter_float(0, 0.0f);                // CH0
				delay_dsp->set_parameter_float(1, time_pullout);       // CH1
				// 设置最大延迟为 50 ms（确保足够容纳 time_pullout）
				delay_dsp->set_parameter_float(8, 50.0f);              // Max Delay

				bus->add_dsp(-1, delay_dsp);
				dsp_chain.push_back(delay_dsp);
			}
		}

		// 2. 创建 PAN DSP 实现声像拉开和环绕效果
		Ref<FmodDSP> pan_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_PAN);
		if (pan_dsp.is_null()) {
			// 如果 PAN DSP 创建失败，则放弃（但可能已有 delay，这里简单返回）
			return;
		}

		// 设置 PAN DSP 参数
		// 参数 0: Panning Mode -> 设置为 2 (Surround) 以启用环绕参数
		pan_dsp->set_parameter_int(0, 2);
		// 参数 6: Stereo-Surround -> 始终启用 (1)
		pan_dsp->set_parameter_int(6, 1);
		// 参数 7: Surr. Stereo Se (立体声分离角度) 映射 pan_pullout: 0-4 -> 0-180 度
		float se = pan_pullout * 45.0f;
		pan_dsp->set_parameter_float(7, se);
		// 参数 3: Surr. Extent (环绕范围) 映射 surround: 0-1 -> 0-360 度
		float extent = surround * 360.0f;
		pan_dsp->set_parameter_float(3, extent);
		// 其他参数保持默认即可

		bus->add_dsp(-1, pan_dsp);
		dsp_chain.push_back(pan_dsp);
	}

	void FmodAudioEffectStereoEnhance::set_pan_pullout(float p_amount) {
		pan_pullout = p_amount;
	}

	float FmodAudioEffectStereoEnhance::get_pan_pullout() const {
		return pan_pullout;
	}

	void FmodAudioEffectStereoEnhance::set_time_pullout(float p_amount) {
		time_pullout = p_amount;
	}

	float FmodAudioEffectStereoEnhance::get_time_pullout() const {
		return time_pullout;
	}

	void FmodAudioEffectStereoEnhance::set_surround(float p_amount) {
		surround = p_amount;
	}

	float FmodAudioEffectStereoEnhance::get_surround() const {
		return surround;
	}
}