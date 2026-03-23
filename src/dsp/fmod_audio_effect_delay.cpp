#include "fmod_audio_effect_delay.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"

namespace godot {
	void FmodAudioEffectDelay::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_dry", "amount"), &FmodAudioEffectDelay::set_dry);
		ClassDB::bind_method(D_METHOD("get_dry"), &FmodAudioEffectDelay::get_dry);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dry", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_dry", "get_dry");

		ClassDB::bind_method(D_METHOD("set_tap1_active", "active"), &FmodAudioEffectDelay::set_tap1_active);
		ClassDB::bind_method(D_METHOD("is_tap1_active"), &FmodAudioEffectDelay::is_tap1_active);

		ClassDB::bind_method(D_METHOD("set_tap1_delay_ms", "delay_ms"), &FmodAudioEffectDelay::set_tap1_delay_ms);
		ClassDB::bind_method(D_METHOD("get_tap1_delay_ms"), &FmodAudioEffectDelay::get_tap1_delay_ms);

		ClassDB::bind_method(D_METHOD("set_tap1_level_db", "level_db"), &FmodAudioEffectDelay::set_tap1_level_db);
		ClassDB::bind_method(D_METHOD("get_tap1_level_db"), &FmodAudioEffectDelay::get_tap1_level_db);

		ClassDB::bind_method(D_METHOD("set_tap1_pan", "pan"), &FmodAudioEffectDelay::set_tap1_pan);
		ClassDB::bind_method(D_METHOD("get_tap1_pan"), &FmodAudioEffectDelay::get_tap1_pan);

		ClassDB::bind_method(D_METHOD("set_tap2_active", "active"), &FmodAudioEffectDelay::set_tap2_active);
		ClassDB::bind_method(D_METHOD("is_tap2_active"), &FmodAudioEffectDelay::is_tap2_active);

		ClassDB::bind_method(D_METHOD("set_tap2_delay_ms", "delay_ms"), &FmodAudioEffectDelay::set_tap2_delay_ms);
		ClassDB::bind_method(D_METHOD("get_tap2_delay_ms"), &FmodAudioEffectDelay::get_tap2_delay_ms);

		ClassDB::bind_method(D_METHOD("set_tap2_level_db", "level_db"), &FmodAudioEffectDelay::set_tap2_level_db);
		ClassDB::bind_method(D_METHOD("get_tap2_level_db"), &FmodAudioEffectDelay::get_tap2_level_db);

		ClassDB::bind_method(D_METHOD("set_tap2_pan", "pan"), &FmodAudioEffectDelay::set_tap2_pan);
		ClassDB::bind_method(D_METHOD("get_tap2_pan"), &FmodAudioEffectDelay::get_tap2_pan);

		ClassDB::bind_method(D_METHOD("set_feedback_active", "active"), &FmodAudioEffectDelay::set_feedback_active);
		ClassDB::bind_method(D_METHOD("is_feedback_active"), &FmodAudioEffectDelay::is_feedback_active);

		ClassDB::bind_method(D_METHOD("set_feedback_delay_ms", "delay_ms"), &FmodAudioEffectDelay::set_feedback_delay_ms);
		ClassDB::bind_method(D_METHOD("get_feedback_delay_ms"), &FmodAudioEffectDelay::get_feedback_delay_ms);

		ClassDB::bind_method(D_METHOD("set_feedback_level_db", "level_db"), &FmodAudioEffectDelay::set_feedback_level_db);
		ClassDB::bind_method(D_METHOD("get_feedback_level_db"), &FmodAudioEffectDelay::get_feedback_level_db);

		ClassDB::bind_method(D_METHOD("set_feedback_lowpass", "lowpass"), &FmodAudioEffectDelay::set_feedback_lowpass);
		ClassDB::bind_method(D_METHOD("get_feedback_lowpass"), &FmodAudioEffectDelay::get_feedback_lowpass);

		ADD_GROUP("Tap 1", "tap1_");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tap1_active"), "set_tap1_active", "is_tap1_active");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "tap1_delay_ms", PROPERTY_HINT_RANGE, "0,10000,1,suffix:ms"), "set_tap1_delay_ms", "get_tap1_delay_ms");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "tap1_level_db", PROPERTY_HINT_RANGE, "-60,0,0.1,suffix:dB"), "set_tap1_level_db", "get_tap1_level_db");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "tap1_pan", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_tap1_pan", "get_tap1_pan");

		ADD_GROUP("Tap 2", "tap2_");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tap2_active"), "set_tap2_active", "is_tap2_active");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "tap2_delay_ms", PROPERTY_HINT_RANGE, "0,10000,1,suffix:ms"), "set_tap2_delay_ms", "get_tap2_delay_ms");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "tap2_level_db", PROPERTY_HINT_RANGE, "-60,0,0.1,suffix:dB"), "set_tap2_level_db", "get_tap2_level_db");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "tap2_pan", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_tap2_pan", "get_tap2_pan");

		ADD_GROUP("Feedback", "feedback_");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "feedback_active"), "set_feedback_active", "is_feedback_active");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "feedback_delay_ms", PROPERTY_HINT_RANGE, "0,10000,1,suffix:ms"), "set_feedback_delay_ms", "get_feedback_delay_ms");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "feedback_level_db", PROPERTY_HINT_RANGE, "-60,0,0.1,suffix:dB"), "set_feedback_level_db", "get_feedback_level_db");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "feedback_lowpass", PROPERTY_HINT_RANGE, "1,16000,1,suffix:Hz"), "set_feedback_lowpass", "get_feedback_lowpass");
	}

	FmodAudioEffectDelay::FmodAudioEffectDelay() {
		dry = 1.0f;

		tap_1_active = true;
		tap_1_delay_ms = 250.0f;
		tap_1_level = -6.0f;
		tap_1_pan = 0.2f;

		tap_2_active = true;
		tap_2_delay_ms = 500.0f;
		tap_2_level = -12.0f;
		tap_2_pan = -0.4f;

		feedback_active = false;
		feedback_delay_ms = 340.0f;
		feedback_level = -6.0f;
		feedback_lowpass = 16000.0f;
	}

	FmodAudioEffectDelay::~FmodAudioEffectDelay() {
	}

	void FmodAudioEffectDelay::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		// 如果已经应用过，先移除
		if (bus.is_valid()) {
			remove_from_bus(bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		// 计算总电平用于归一化
		float tap1_linear = tap_1_active ? FmodUtils::db_to_linear(tap_1_level) : 0.0f;
		float tap2_linear = tap_2_active ? FmodUtils::db_to_linear(tap_2_level) : 0.0f;
		float total_wet = tap1_linear + tap2_linear;
		float total_signal = dry + total_wet;

		// 如果总信号超过 1.0，进行归一化
		// 这防止多个抽头叠加导致音量过大
		float normalize_scale = (total_signal > 1.0f) ? (1.0f / total_signal) : 1.0f;

		// 处理抽头：第一个活动的抽头负责干/湿混合
		bool first_tap = true;

		// Tap 1
		if (tap_1_active) {
			_create_tap_dsp(system, tap_1_delay_ms, tap_1_level, tap_1_pan, normalize_scale, first_tap);
			first_tap = false;
		}

		// Tap 2
		if (tap_2_active) {
			_create_tap_dsp(system, tap_2_delay_ms, tap_2_level, tap_2_pan, normalize_scale, first_tap);
			first_tap = false;
		}

		// Feedback
		if (feedback_active) {
			_create_feedback_dsp(system, feedback_delay_ms, feedback_level, feedback_lowpass);
		}
	}

	void FmodAudioEffectDelay::_create_tap_dsp(Ref<FmodSystem> system, float delay_ms, float level_db, float pan, float normalize_scale, bool is_first_tap) {
		// 创建 Delay DSP
		Ref<FmodDSP> delay_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_DELAY);
		if (!delay_dsp.is_valid()) return;

		// 设置 Max delay
		float max_delay = MAX(delay_ms + 10.0f, delay_ms * 1.1f);
		max_delay = CLAMP(max_delay, delay_ms, 10000.0f);
		delay_dsp->set_parameter_float(16, max_delay);

		// 设置所有 16 个通道的延迟
		for (int i = 0; i < 16; i++) {
			delay_dsp->set_parameter_float(i, delay_ms);
		}

		// 计算电平：dB 转线性，然后应用归一化
		float level_linear = FmodUtils::db_to_linear(level_db) * normalize_scale;

		// FMOD Delay DSP: set_wet_dry_mix(prewet, postwet, dry)
		// prewet: 输入到湿信号的比例 (0-1)
		// postwet: 湿信号输出比例 (0-1)  
		// dry: 干信号通过比例 (0-1)
		// 
		// 第一个 tap 负责干/湿混合，后续 tap 只加湿信号
		float dry_level = is_first_tap ? dry : 0.0f;
		delay_dsp->set_wet_dry_mix(1.0f, level_linear, dry_level);

		bus->add_dsp(-1, delay_dsp);
		dsp_chain.push_back(delay_dsp);

		// 声像处理
		if (pan != 0.0f) {
			Ref<FmodDSP> pan_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_PAN);
			if (pan_dsp.is_valid()) {
				pan_dsp->set_parameter_float(1, pan * 100.0f);
				pan_dsp->set_wet_dry_mix(1.0f, 1.0f, 1.0f);
				bus->add_dsp(-1, pan_dsp);
				dsp_chain.push_back(pan_dsp);
			}
		}
	}

	void FmodAudioEffectDelay::_create_feedback_dsp(Ref<FmodSystem> system, float delay_ms, float level_db, float lowpass_hz) {
		Ref<FmodDSP> delay_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_DELAY);
		if (!delay_dsp.is_valid()) return;

		float max_delay = MAX(delay_ms + 10.0f, delay_ms * 1.1f);
		max_delay = CLAMP(max_delay, delay_ms, 10000.0f);
		delay_dsp->set_parameter_float(16, max_delay);

		for (int i = 0; i < 16; i++) {
			delay_dsp->set_parameter_float(i, delay_ms);
		}

		float feedback_linear = FmodUtils::db_to_linear(level_db);
		delay_dsp->set_wet_dry_mix(1.0f, feedback_linear, 1.0f);

		bus->add_dsp(-1, delay_dsp);
		dsp_chain.push_back(delay_dsp);

		if (lowpass_hz < 16000.0f) {
			Ref<FmodDSP> lowpass_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_LOWPASS_SIMPLE);
			if (lowpass_dsp.is_valid()) {
				lowpass_dsp->set_parameter_float(0, lowpass_hz);
				bus->add_dsp(-1, lowpass_dsp);
				dsp_chain.push_back(lowpass_dsp);
			}
		}
	}

	void FmodAudioEffectDelay::set_dry(float p_dry) {
		dry = CLAMP(p_dry, 0.0f, 1.0f);
	}

	float FmodAudioEffectDelay::get_dry() {
		return dry;
	}

	void FmodAudioEffectDelay::set_tap1_active(bool p_active) {
		tap_1_active = p_active;
	}

	bool FmodAudioEffectDelay::is_tap1_active() const {
		return tap_1_active;
	}

	void FmodAudioEffectDelay::set_tap1_delay_ms(float p_delay_ms) {
		tap_1_delay_ms = CLAMP(p_delay_ms, 0.0f, 10000.0f);
	}

	float FmodAudioEffectDelay::get_tap1_delay_ms() const {
		return tap_1_delay_ms;
	}

	void FmodAudioEffectDelay::set_tap1_level_db(float p_level_db) {
		tap_1_level = CLAMP(p_level_db, -60.0f, 0.0f);
	}

	float FmodAudioEffectDelay::get_tap1_level_db() const {
		return tap_1_level;
	}

	void FmodAudioEffectDelay::set_tap1_pan(float p_pan) {
		tap_1_pan = CLAMP(p_pan, -1.0f, 1.0f);
	}

	float FmodAudioEffectDelay::get_tap1_pan() const {
		return tap_1_pan;
	}

	void FmodAudioEffectDelay::set_tap2_active(bool p_active) {
		tap_2_active = p_active;
	}

	bool FmodAudioEffectDelay::is_tap2_active() const {
		return tap_2_active;
	}

	void FmodAudioEffectDelay::set_tap2_delay_ms(float p_delay_ms) {
		tap_2_delay_ms = CLAMP(p_delay_ms, 0.0f, 10000.0f);
	}

	float FmodAudioEffectDelay::get_tap2_delay_ms() const {
		return tap_2_delay_ms;
	}

	void FmodAudioEffectDelay::set_tap2_level_db(float p_level_db) {
		tap_2_level = CLAMP(p_level_db, -60.0f, 0.0f);
	}

	float FmodAudioEffectDelay::get_tap2_level_db() const {
		return tap_2_level;
	}

	void FmodAudioEffectDelay::set_tap2_pan(float p_pan) {
		tap_2_pan = CLAMP(p_pan, -1.0f, 1.0f);
	}

	float FmodAudioEffectDelay::get_tap2_pan() const {
		return tap_2_pan;
	}

	void FmodAudioEffectDelay::set_feedback_active(bool p_active) {
		feedback_active = p_active;
	}

	bool FmodAudioEffectDelay::is_feedback_active() const {
		return feedback_active;
	}

	void FmodAudioEffectDelay::set_feedback_delay_ms(float p_delay_ms) {
		feedback_delay_ms = CLAMP(p_delay_ms, 0.0f, 10000.0f);
	}

	float FmodAudioEffectDelay::get_feedback_delay_ms() const {
		return feedback_delay_ms;
	}

	void FmodAudioEffectDelay::set_feedback_level_db(float p_level_db) {
		feedback_level = CLAMP(p_level_db, -60.0f, 0.0f);
	}

	float FmodAudioEffectDelay::get_feedback_level_db() const {
		return feedback_level;
	}

	void FmodAudioEffectDelay::set_feedback_lowpass(float p_lowpass) {
		feedback_lowpass = CLAMP(p_lowpass, 1.0f, 16000.0f);
	}

	float FmodAudioEffectDelay::get_feedback_lowpass() const {
		return feedback_lowpass;
	}
}
