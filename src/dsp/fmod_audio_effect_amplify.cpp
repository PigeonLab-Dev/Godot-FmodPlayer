#include "dsp/fmod_audio_effect_amplify.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"

namespace godot {
	void FmodAudioEffectAmplify::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_volume_db", "volume_db"), &FmodAudioEffectAmplify::set_volume_db);
		ClassDB::bind_method(D_METHOD("get_volume_db"), &FmodAudioEffectAmplify::get_volume_db);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume_db", PROPERTY_HINT_RANGE, "-80,24,0.1,suffix:dB"), "set_volume_db", "get_volume_db");
		
		ClassDB::bind_method(D_METHOD("set_volume_linear", "volume_linear"), &FmodAudioEffectAmplify::set_volume_linear);
		ClassDB::bind_method(D_METHOD("get_volume_linear"), &FmodAudioEffectAmplify::get_volume_linear);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume_linear"), "set_volume_linear", "get_volume_linear");
	}

	FmodAudioEffectAmplify::FmodAudioEffectAmplify() {

	}

	FmodAudioEffectAmplify::~FmodAudioEffectAmplify() {

	}

	void FmodAudioEffectAmplify::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		// 如果已经应用到了同一个 bus，先移除旧的
		if (bus == p_bus && dsp_chain.size() > 0) {
			remove_from_bus(p_bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		if (system.is_null()) return;

		// FMOD Fader DSP 的 Gain 参数范围是 -80 ~ 10 dB
		// 为了达到 Godot 的 -80 ~ 24 dB 范围，使用两个 Fader DSP 串联
		// Fader 1: 处理 -80 ~ 10 dB 的基础增益
		// Fader 2: 当需要超过 10 dB 时，处理额外的增益 (0 ~ 14 dB)

		// 计算两个 fader 的增益分配
		float fader1_gain = Math::clamp(volume_db, -80.0f, 10.0f);
		float fader2_gain = (volume_db > 10.0f) ? (volume_db - 10.0f) : 0.0f;

		// 创建第一个 Fader DSP (基础增益)
		Ref<FmodDSP> fader1 = system->create_dsp_by_type(FmodDSP::DSP_TYPE_FADER);
		if (!fader1.is_valid()) {
			return;
		}
		fader1->set_parameter_float(0, fader1_gain);			// 参数 0 = Gain in dB
		bus->add_dsp(-1, fader1);
		dsp_chain.push_back(fader1);

		// 如果需要额外增益 (>10dB)，创建第二个 Fader DSP
		if (fader2_gain > 0.0f) {
			Ref<FmodDSP> fader2 = system->create_dsp_by_type(FmodDSP::DSP_TYPE_FADER);
			if (fader2.is_valid()) {
				fader2->set_parameter_float(0, fader2_gain);	// 参数 0 = Gain in dB
				bus->add_dsp(-1, fader2);
				dsp_chain.push_back(fader2);
			}
		}
	}

	void FmodAudioEffectAmplify::set_volume_db(float p_volume_db) {
		volume_db = CLAMP(p_volume_db, -80.0f, 24.0f);
		
		// 如果已经应用到 bus，实时更新 DSP 参数
		_update_dsp_gains();
	}

	float FmodAudioEffectAmplify::get_volume_db() const {
		return volume_db;
	}

	void FmodAudioEffectAmplify::set_volume_linear(float p_volume_linear) {
		// 线性值转换为 dB
		float db = FmodUtils::linear_to_db(p_volume_linear);
		set_volume_db(db);
	}

	float FmodAudioEffectAmplify::get_volume_linear() const {
		// dB 转换为线性值
		return FmodUtils::db_to_linear(volume_db);
	}

	void FmodAudioEffectAmplify::_update_dsp_gains() {
		if (dsp_chain.size() == 0) {
			return;
		}

		// 计算两个 fader 的增益分配
		float fader1_gain = Math::clamp(volume_db, -80.0f, 10.0f);
		float fader2_gain = (volume_db > 10.0f) ? (volume_db - 10.0f) : 0.0f;

		// 更新第一个 Fader (总是存在)
		if (dsp_chain[0].is_valid()) {
			dsp_chain[0]->set_parameter_float(0, fader1_gain);
		}

		// 更新或创建第二个 Fader
		if (fader2_gain > 0.0f) {
			if (dsp_chain.size() >= 2 && dsp_chain[1].is_valid()) {
				// 更新第二个 fader 的增益
				dsp_chain[1]->set_parameter_float(0, fader2_gain);
			} else if (dsp_chain.size() == 1 && dsp_chain[0].is_valid()) {
				// 需要创建第二个 fader
				Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
				if (system.is_valid()) {
					Ref<FmodDSP> fader2 = system->create_dsp_by_type(FmodDSP::DSP_TYPE_FADER);
					if (fader2.is_valid()) {
						fader2->set_parameter_float(0, fader2_gain);
						bus->add_dsp(-1, fader2);
						dsp_chain.push_back(fader2);
					}
				}
			}
		} else {
			// 不需要第二个 fader，如果存在则移除
			if (dsp_chain.size() >= 2) {
				if (dsp_chain[1].is_valid() && bus.is_valid()) {
					bus->remove_dsp(dsp_chain[1]);
				}
				dsp_chain.resize(1);
			}
		}
	}
}
