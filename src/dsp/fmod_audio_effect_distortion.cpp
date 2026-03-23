#include "dsp/fmod_audio_effect_distortion.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"

namespace godot {
	void FmodAudioEffectDistortion::_bind_methods() {
		BIND_ENUM_CONSTANT(MODE_CLIP);
		BIND_ENUM_CONSTANT(MODE_ATAN);
		BIND_ENUM_CONSTANT(MODE_LOFI);
		BIND_ENUM_CONSTANT(MODE_OVERDRIVE);
		BIND_ENUM_CONSTANT(MODE_WAVESHAPE);
		BIND_ENUM_CONSTANT(MODE_BITCRUSH);

		ClassDB::bind_method(D_METHOD("set_mode", "mode"), &FmodAudioEffectDistortion::set_mode);
		ClassDB::bind_method(D_METHOD("get_mode"), &FmodAudioEffectDistortion::get_mode);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "mode", PROPERTY_HINT_ENUM, "Clip,Atan,LoFi,Overdrive,Waveshape,BitCrush"), "set_mode", "get_mode");

		ClassDB::bind_method(D_METHOD("set_pre_gain", "gain"), &FmodAudioEffectDistortion::set_pre_gain);
		ClassDB::bind_method(D_METHOD("get_pre_gain"), &FmodAudioEffectDistortion::get_pre_gain);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pre_gain", PROPERTY_HINT_RANGE, "-60,24,0.1,suffix:dB"), "set_pre_gain", "get_pre_gain");

		ClassDB::bind_method(D_METHOD("set_drive", "drive"), &FmodAudioEffectDistortion::set_drive);
		ClassDB::bind_method(D_METHOD("get_drive"), &FmodAudioEffectDistortion::get_drive);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "drive", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_drive", "get_drive");

		ClassDB::bind_method(D_METHOD("set_post_gain", "gain"), &FmodAudioEffectDistortion::set_post_gain);
		ClassDB::bind_method(D_METHOD("get_post_gain"), &FmodAudioEffectDistortion::get_post_gain);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "post_gain", PROPERTY_HINT_RANGE, "-60,24,0.1,suffix:dB"), "set_post_gain", "get_post_gain");

		ClassDB::bind_method(D_METHOD("set_keep_hf_hz", "hz"), &FmodAudioEffectDistortion::set_keep_hf_hz);
		ClassDB::bind_method(D_METHOD("get_keep_hf_hz"), &FmodAudioEffectDistortion::get_keep_hf_hz);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "keep_hf_hz", PROPERTY_HINT_RANGE, "1000,20000,1,suffix:Hz"), "set_keep_hf_hz", "get_keep_hf_hz");

		ClassDB::bind_method(D_METHOD("set_oversample", "oversample"), &FmodAudioEffectDistortion::set_oversample);
		ClassDB::bind_method(D_METHOD("get_oversample"), &FmodAudioEffectDistortion::get_oversample);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "oversample", PROPERTY_HINT_RANGE, "1,16,1"), "set_oversample", "get_oversample");
	}

	FmodAudioEffectDistortion::FmodAudioEffectDistortion() {

	}

	FmodAudioEffectDistortion::~FmodAudioEffectDistortion() {
		
	}

	void FmodAudioEffectDistortion::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		// 如果已经应用到了同一个 bus，先移除旧的
		if (bus == p_bus && dsp_chain.size() > 0) {
			remove_from_bus(p_bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		// 前置增益 (如果需要)
		if (pre_gain != 0.0f) {
			Ref<FmodDSP> gain_dsp = _create_gain_dsp(system, pre_gain);
			if (gain_dsp.is_valid()) {
				bus->add_dsp(-1, gain_dsp);
				dsp_chain.push_back(gain_dsp);
			}
		}

		// 高频保护滤波器 (当 drive 较高时使用)
		if (keep_hf_hz < 20000.0f && drive > 0.3f) {
			Ref<FmodDSP> lowpass = system->create_dsp_by_type(FmodDSP::DSP_TYPE_LOWPASS_SIMPLE);
			if (lowpass.is_valid()) {
				// FMOD 低通滤波器参数：截止频率 (Hz)
				lowpass->set_parameter_float(0, keep_hf_hz);
				bus->add_dsp(-1, lowpass);
				dsp_chain.push_back(lowpass);
			}
		}

		// 主失真 DSP (根据模式选择)
		Ref<FmodDSP> distortion_dsp;
		switch (mode) {
		case MODE_LOFI:
			distortion_dsp = _create_lofi_dsp(system);
			break;
		case MODE_BITCRUSH:
			distortion_dsp = _create_bitcrush_dsp(system);
			break;
		case MODE_WAVESHAPE:
			distortion_dsp = _create_waveshape_dsp(system);
			break;
		case MODE_CLIP:
		case MODE_ATAN:
		case MODE_OVERDRIVE:
		default:
			distortion_dsp = _create_distortion_dsp(system);
			break;
		}

		if (distortion_dsp.is_valid()) {
			bus->add_dsp(-1, distortion_dsp);
			dsp_chain.push_back(distortion_dsp);
		}

		// 后置增益 (如果需要)
		if (post_gain != 0.0f) {
			Ref<FmodDSP> gain_dsp = _create_gain_dsp(system, post_gain);
			if (gain_dsp.is_valid()) {
				bus->add_dsp(-1, gain_dsp);
				dsp_chain.push_back(gain_dsp);
			}
		}
	}

	Ref<FmodDSP> FmodAudioEffectDistortion::_create_distortion_dsp(Ref<FmodSystem> system) {
		Ref<FmodDSP> dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_DISTORTION);
		if (!dsp.is_valid()) {
			return Ref<FmodDSP>();
		}

		// 根据模式调整参数
		// drive 0.0 ~ 1.0 映射到 level 0.0 ~ 1.0
		// drive=0 时应该无失真，drive=1 时最大失真
		switch (mode) {
		case MODE_CLIP:
			// 硬剪辑
			dsp->set_parameter_float(0, drive);	// level
			break;
		case MODE_ATAN:
			// 反正切软剪辑
			dsp->set_parameter_float(0, drive);	// level
			break;
		case MODE_OVERDRIVE:
			// 过载：模拟电子管
			dsp->set_parameter_float(0, drive);	// level
			break;
		default:
			dsp->set_parameter_float(0, drive);	// level
			break;
		}

		return dsp;
	}

	Ref<FmodDSP> FmodAudioEffectDistortion::_create_gain_dsp(Ref<FmodSystem> system, float gain_db) {
		// 使用 Fader DSP 作为增益控制
		Ref<FmodDSP> dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_FADER);
		if (!dsp.is_valid()) {
			return Ref<FmodDSP>();
		}

		// 将 dB 转换为线性增益，并设置音量
		float linear_gain = FmodUtils::db_to_linear(gain_db);
		// Fader DSP 参数: 0 = gain
		dsp->set_parameter_float(0, linear_gain);

		return dsp;
	}

	Ref<FmodDSP> FmodAudioEffectDistortion::_create_lofi_dsp(Ref<FmodSystem> system) {
		// LoFi 效果：使用失真 + 降采样模拟
		Ref<FmodDSP> dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_DISTORTION);
		if (!dsp.is_valid()) {
			return Ref<FmodDSP>();
		}

		// LoFi：drive 映射到 level 0.0 ~ 1.0
		dsp->set_parameter_float(0, drive);

		// TODO: 如果需要更精确的 LoFi 效果（降采样），可以添加自定义 DSP

		return dsp;
	}

	Ref<FmodDSP> FmodAudioEffectDistortion::_create_bitcrush_dsp(Ref<FmodSystem> system) {
		// 比特破碎：使用失真模拟
		Ref<FmodDSP> dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_DISTORTION);
		if (!dsp.is_valid()) {
			return Ref<FmodDSP>();
		}

		// BitCrush：drive 映射到 level 0.0 ~ 1.0
		dsp->set_parameter_float(0, drive);

		return dsp;
	}

	Ref<FmodDSP> FmodAudioEffectDistortion::_create_waveshape_dsp(Ref<FmodSystem> system) {
		// 波形塑形：使用失真
		Ref<FmodDSP> dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_DISTORTION);
		if (!dsp.is_valid()) {
			return Ref<FmodDSP>();
		}

		// Waveshape：drive 映射到 level 0.0 ~ 1.0
		dsp->set_parameter_float(0, drive);

		return dsp;
	}

	// Getters/Setters
	void FmodAudioEffectDistortion::set_mode(Mode p_mode) {
		mode = p_mode;
	}

	FmodAudioEffectDistortion::Mode FmodAudioEffectDistortion::get_mode() const {
		return mode;
	}

	void FmodAudioEffectDistortion::set_pre_gain(float p_gain) {
		pre_gain = CLAMP(p_gain, -60.0f, 24.0f);
	}

	float FmodAudioEffectDistortion::get_pre_gain() const {
		return pre_gain;
	}

	void FmodAudioEffectDistortion::set_drive(float p_drive) {
		drive = CLAMP(p_drive, 0.0f, 1.0f);
	}

	float FmodAudioEffectDistortion::get_drive() const {
		return drive;
	}

	void FmodAudioEffectDistortion::set_post_gain(float p_gain) {
		post_gain = CLAMP(p_gain, -60.0f, 24.0f);
	}

	float FmodAudioEffectDistortion::get_post_gain() const {
		return post_gain;
	}

	void FmodAudioEffectDistortion::set_keep_hf_hz(float p_hz) {
		keep_hf_hz = CLAMP(p_hz, 1000.0f, 20000.0f);
	}

	float FmodAudioEffectDistortion::get_keep_hf_hz() const {
		return keep_hf_hz;
	}

	void FmodAudioEffectDistortion::set_oversample(int p_oversample) {
		oversample = CLAMP(p_oversample, 1, 16);
	}

	int FmodAudioEffectDistortion::get_oversample() const {
		return oversample;
	}

} // namespace godot
