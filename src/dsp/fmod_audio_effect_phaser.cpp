#include "dsp/fmod_audio_effect_phaser.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {

	void FmodAudioEffectPhaser::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_depth", "depth"), &FmodAudioEffectPhaser::set_depth);
		ClassDB::bind_method(D_METHOD("get_depth"), &FmodAudioEffectPhaser::get_depth);
		ClassDB::bind_method(D_METHOD("set_feedback", "feedback"), &FmodAudioEffectPhaser::set_feedback);
		ClassDB::bind_method(D_METHOD("get_feedback"), &FmodAudioEffectPhaser::get_feedback);
		ClassDB::bind_method(D_METHOD("set_range_max_hz", "range_max_hz"), &FmodAudioEffectPhaser::set_range_max_hz);
		ClassDB::bind_method(D_METHOD("get_range_max_hz"), &FmodAudioEffectPhaser::get_range_max_hz);
		ClassDB::bind_method(D_METHOD("set_range_min_hz", "range_min_hz"), &FmodAudioEffectPhaser::set_range_min_hz);
		ClassDB::bind_method(D_METHOD("get_range_min_hz"), &FmodAudioEffectPhaser::get_range_min_hz);
		ClassDB::bind_method(D_METHOD("set_rate_hz", "rate_hz"), &FmodAudioEffectPhaser::set_rate_hz);
		ClassDB::bind_method(D_METHOD("get_rate_hz"), &FmodAudioEffectPhaser::get_rate_hz);

		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "depth", PROPERTY_HINT_RANGE, "0.1,4.0,0.01"), "set_depth", "get_depth");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "feedback", PROPERTY_HINT_RANGE, "0.1,0.9,0.01"), "set_feedback", "get_feedback");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "range_max_hz", PROPERTY_HINT_RANGE, "10,10000,1,suffix:Hz"), "set_range_max_hz", "get_range_max_hz");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "range_min_hz", PROPERTY_HINT_RANGE, "10,10000,1,suffix:Hz"), "set_range_min_hz", "get_range_min_hz");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "rate_hz", PROPERTY_HINT_RANGE, "0.5,20.0,0.01,suffix:Hz"), "set_rate_hz", "get_rate_hz");
	}

	FmodAudioEffectPhaser::FmodAudioEffectPhaser() {
	}

	FmodAudioEffectPhaser::~FmodAudioEffectPhaser() {
	}

	void FmodAudioEffectPhaser::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		// 如果已经应用到了同一个 bus，先移除旧的
		if (bus == p_bus && !dsp_chain.is_empty()) {
			remove_from_bus(p_bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		// 创建 FLANGE DSP 来近似模拟 Phaser 效果
		Ref<FmodDSP> phaser_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_FLANGE);
		if (!phaser_dsp.is_valid()) {
			return;
		}

		// FMOD FLANGE DSP 参数：
		// 0 = Mix (float, %) - 干湿混合比例，范围 0.0 ~ 100.0，默认 50.0
		// 1 = Depth (float) - 镶边深度，范围 0.01 ~ 1.0，默认 1.0
		// 2 = Rate (float, Hz) - 镶边速度，范围 0.0 ~ 20.0，默认 0.1

		// 参数映射：
		// Mix: 使用 feedback 映射到 Mix (0.1~0.9 -> 10%~90%)
		// Depth: Godot depth (0.1~4.0) 映射到 FMOD Depth (0.01~1.0)
		// Rate: 直接映射 (Hz)

		// 设置 Mix (feedback -> %)
		float mix_percent = feedback * 100.0f;
		phaser_dsp->set_parameter_float(0, mix_percent);

		// 设置 Depth (缩放映射)
		// Godot: 0.1 ~ 4.0, FMOD: 0.01 ~ 1.0
		float fmod_depth = CLAMP(depth / 4.0f, 0.01f, 1.0f);
		phaser_dsp->set_parameter_float(1, fmod_depth);

		// 设置 Rate (直接映射)
		phaser_dsp->set_parameter_float(2, rate_hz);

		bus->add_dsp(-1, phaser_dsp);
		dsp_chain.push_back(phaser_dsp);
	}

	void FmodAudioEffectPhaser::set_depth(float p_depth) {
		depth = CLAMP(p_depth, 0.1f, 4.0f);
	}

	float FmodAudioEffectPhaser::get_depth() const {
		return depth;
	}

	void FmodAudioEffectPhaser::set_feedback(float p_feedback) {
		feedback = CLAMP(p_feedback, 0.1f, 0.9f);
	}

	float FmodAudioEffectPhaser::get_feedback() const {
		return feedback;
	}

	void FmodAudioEffectPhaser::set_range_max_hz(float p_range_max_hz) {
		// 确保最大值不小于最小值
		range_max_hz = CLAMP(p_range_max_hz, range_min_hz, 10000.0f);
	}

	float FmodAudioEffectPhaser::get_range_max_hz() const {
		return range_max_hz;
	}

	void FmodAudioEffectPhaser::set_range_min_hz(float p_range_min_hz) {
		// 确保最小值不大于最大值
		range_min_hz = CLAMP(p_range_min_hz, 10.0f, range_max_hz);
	}

	float FmodAudioEffectPhaser::get_range_min_hz() const {
		return range_min_hz;
	}

	void FmodAudioEffectPhaser::set_rate_hz(float p_rate_hz) {
		rate_hz = CLAMP(p_rate_hz, 0.5f, 20.0f);
	}

	float FmodAudioEffectPhaser::get_rate_hz() const {
		return rate_hz;
	}

} // namespace godot
