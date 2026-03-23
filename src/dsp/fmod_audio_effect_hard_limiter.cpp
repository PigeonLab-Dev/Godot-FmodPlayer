#include "dsp/fmod_audio_effect_hard_limiter.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "core/fmod_utils.hpp"
#include "playback/fmod_channel_group.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {

	void FmodAudioEffectHardLimiter::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_ceiling_db", "ceiling_db"), &FmodAudioEffectHardLimiter::set_ceiling_db);
		ClassDB::bind_method(D_METHOD("get_ceiling_db"), &FmodAudioEffectHardLimiter::get_ceiling_db);
		ClassDB::bind_method(D_METHOD("set_pre_gain_db", "pre_gain_db"), &FmodAudioEffectHardLimiter::set_pre_gain_db);
		ClassDB::bind_method(D_METHOD("get_pre_gain_db"), &FmodAudioEffectHardLimiter::get_pre_gain_db);
		ClassDB::bind_method(D_METHOD("set_release", "release"), &FmodAudioEffectHardLimiter::set_release);
		ClassDB::bind_method(D_METHOD("get_release"), &FmodAudioEffectHardLimiter::get_release);

		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ceiling_db", PROPERTY_HINT_RANGE, "-24.0,0.0,0.1,suffix:dB"), "set_ceiling_db", "get_ceiling_db");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pre_gain_db", PROPERTY_HINT_RANGE, "-60.0,24.0,0.1,suffix:dB"), "set_pre_gain_db", "get_pre_gain_db");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "release", PROPERTY_HINT_RANGE, "0.001,1.0,0.001,suffix:s"), "set_release", "get_release");
	}

	FmodAudioEffectHardLimiter::FmodAudioEffectHardLimiter() {
	}

	FmodAudioEffectHardLimiter::~FmodAudioEffectHardLimiter() {
	}

	void FmodAudioEffectHardLimiter::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		// 如果已经应用到了同一个 bus，先移除旧的
		if (bus == p_bus && !dsp_chain.is_empty()) {
			remove_from_bus(p_bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		// 前置增益（如果需要）
		if (pre_gain_db != 0.0f) {
			Ref<FmodDSP> gain_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_FADER);
			if (gain_dsp.is_valid()) {
				float linear_gain = FmodUtils::db_to_linear(pre_gain_db);
				gain_dsp->set_parameter_float(0, linear_gain);
				bus->add_dsp(-1, gain_dsp);
				dsp_chain.push_back(gain_dsp);
			}
		}

		// 创建 Limiter DSP
		Ref<FmodDSP> limiter_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_LIMITER);
		if (!limiter_dsp.is_valid()) {
			return;
		}

		// FMOD Limiter DSP 参数：
		// 0 = Release Time (float, ms) - 范围 1.0 ~ 1000.0, 默认 10.0
		// 1 = Ceiling (float, dB) - 范围 -12.0 ~ 0.0, 默认 0.0
		// 2 = Maximizer Gain (float, dB) - 范围 0.0 ~ 12.0, 默认 0.0
		// 3 = Linked mode (bool) - 默认 false

		// 设置释放时间（秒转毫秒）
		limiter_dsp->set_parameter_float(0, release * 1000.0f);
		// 设置上限 (dB) - FMOD 范围是 -12~0，Godot 是 -24~0，需要截断
		limiter_dsp->set_parameter_float(1, CLAMP(ceiling_db, -12.0f, 0.0f));
		// 设置最大化增益为 0（我们不使用最大化功能）
		limiter_dsp->set_parameter_float(2, 0.0f);
		// 注意：参数 3 是 bool 类型的 Linked mode，不需要设置，保持默认值 false 即可

		bus->add_dsp(-1, limiter_dsp);
		dsp_chain.push_back(limiter_dsp);
	}

	void FmodAudioEffectHardLimiter::set_ceiling_db(float p_ceiling_db) {
		ceiling_db = CLAMP(p_ceiling_db, -24.0f, 0.0f);
	}

	float FmodAudioEffectHardLimiter::get_ceiling_db() const {
		return ceiling_db;
	}

	void FmodAudioEffectHardLimiter::set_pre_gain_db(float p_pre_gain_db) {
		pre_gain_db = CLAMP(p_pre_gain_db, -60.0f, 24.0f);
	}

	float FmodAudioEffectHardLimiter::get_pre_gain_db() const {
		return pre_gain_db;
	}

	void FmodAudioEffectHardLimiter::set_release(float p_release) {
		release = CLAMP(p_release, 0.001f, 1.0f);
	}

	float FmodAudioEffectHardLimiter::get_release() const {
		return release;
	}

} // namespace godot
