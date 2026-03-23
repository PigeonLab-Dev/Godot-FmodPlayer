#include "dsp/fmod_audio_effect_filter.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"

namespace godot {
	void FmodAudioEffectFilter::_bind_methods() {
		BIND_ENUM_CONSTANT(FILTER_6DB);
		BIND_ENUM_CONSTANT(FILTER_12DB);
		BIND_ENUM_CONSTANT(FILTER_18DB);
		BIND_ENUM_CONSTANT(FILTER_24DB);

		ClassDB::bind_method(D_METHOD("set_cutoff_hz", "cutoff_hz"), &FmodAudioEffectFilter::set_cutoff_hz);
		ClassDB::bind_method(D_METHOD("get_cutoff_hz"), &FmodAudioEffectFilter::get_cutoff_hz);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cutoff_hz", PROPERTY_HINT_RANGE, "1,20500,1,suffix:Hz"), "set_cutoff_hz", "get_cutoff_hz");

		ClassDB::bind_method(D_METHOD("set_db", "db"), &FmodAudioEffectFilter::set_db);
		ClassDB::bind_method(D_METHOD("get_db"), &FmodAudioEffectFilter::get_db);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "db", PROPERTY_HINT_ENUM, "6 dB,12 dB,18 dB,24 dB"), "set_db", "get_db");

		ClassDB::bind_method(D_METHOD("set_gain", "gain"), &FmodAudioEffectFilter::set_gain);
		ClassDB::bind_method(D_METHOD("get_gain"), &FmodAudioEffectFilter::get_gain);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gain", PROPERTY_HINT_RANGE, "0.01,16,0.01"), "set_gain", "get_gain");

		ClassDB::bind_method(D_METHOD("set_resonance", "resonance"), &FmodAudioEffectFilter::set_resonance);
		ClassDB::bind_method(D_METHOD("get_resonance"), &FmodAudioEffectFilter::get_resonance);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "resonance", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_resonance", "get_resonance");
	}

	FmodAudioEffectFilter::FmodAudioEffectFilter() {

	}

	FmodAudioEffectFilter::~FmodAudioEffectFilter() {

	}

	void FmodAudioEffectFilter::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		// 如果已经应用到了同一个 bus，先移除旧的
		if (bus == p_bus && dsp_chain.size() > 0) {
			remove_from_bus(p_bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		// 使用 MULTIBAND_EQ 来实现滤波器
		// 它支持多种滤波器类型和阶数
		Ref<FmodDSP> eq_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_MULTIBAND_EQ);
		if (!eq_dsp.is_valid()) {
			return;
		}

		// 配置滤波器参数
		// MULTIBAND_EQ 有 A/B/C/D/E 五个频段，我们使用 A 频段
		// A_FILTER (参数 0): 滤波器类型
		// A_FREQUENCY (参数 1): 频率
		// A_Q (参数 2): Q值/共振
		// A_GAIN (参数 3): 增益

		// 映射 db 到 FMOD 滤波器类型 (使用低通作为默认)
		// FILTER_6DB  -> LOWPASS_6DB
		// FILTER_12DB -> LOWPASS_12DB
		// FILTER_18DB -> LOWPASS_24DB (FMOD 没有 18dB，用 24dB 近似)
		// FILTER_24DB -> LOWPASS_24DB
		int filter_type = FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_6DB;
		switch (db) {
			case FILTER_6DB:
				filter_type = FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_6DB;
				break;
			case FILTER_12DB:
				filter_type = FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_12DB;
				break;
			case FILTER_18DB:
				// FMOD 没有 18dB 选项，使用 24dB 作为近似
				filter_type = FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_24DB;
				break;
			case FILTER_24DB:
				filter_type = FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_24DB;
				break;
		}

		// Q 值映射：resonance 0.0~1.0 映射到 Q 值 0.1~10.0
		float q_value = 0.1f + resonance * 9.9f;

		// 设置参数
		eq_dsp->set_parameter_int(0, filter_type);									// A_FILTER
		eq_dsp->set_parameter_float(1, cutoff_hz);									// A_FREQUENCY
		eq_dsp->set_parameter_float(2, q_value);									// A_Q
		eq_dsp->set_parameter_float(3, 20.0f * FmodUtils::linear_to_db(gain));		// A_GAIN (转换为 dB)

		// 禁用其他频段 (B/C/D/E)
		for (int i = 4; i < 20; i += 4) {
			eq_dsp->set_parameter_int(i, FMOD_DSP_MULTIBAND_EQ_FILTER_DISABLED);
		}

		// 添加到 bus
		bus->add_dsp(-1, eq_dsp);
		dsp_chain.push_back(eq_dsp);
	}

	void FmodAudioEffectFilter::set_cutoff_hz(float p_cutoff_hz) {
		cutoff_hz = CLAMP(p_cutoff_hz, 1.0f, 22000.0f);
		_update_dsp_params();
	}

	float FmodAudioEffectFilter::get_cutoff_hz() const {
		return cutoff_hz;
	}

	void FmodAudioEffectFilter::set_db(FilterDB p_db) {
		db = p_db;
		_update_dsp_params();
	}

	FmodAudioEffectFilter::FilterDB FmodAudioEffectFilter::get_db() const {
		return db;
	}

	void FmodAudioEffectFilter::set_gain(float p_gain) {
		gain = CLAMP(p_gain, 0.01f, 16.0f);
		_update_dsp_params();
	}

	float FmodAudioEffectFilter::get_gain() const {
		return gain;
	}

	void FmodAudioEffectFilter::set_resonance(float p_resonance) {
		resonance = CLAMP(p_resonance, 0.0f, 1.0f);
		_update_dsp_params();
	}

	float FmodAudioEffectFilter::get_resonance() const {
		return resonance;
	}

	void FmodAudioEffectFilter::_update_dsp_params() {
		if (dsp_chain.size() == 0 || !dsp_chain[0].is_valid()) {
			return;
		}

		Ref<FmodDSP> eq_dsp = dsp_chain[0];

		// 映射 db 到 FMOD 滤波器类型
		int filter_type = FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_6DB;
		switch (db) {
			case FILTER_6DB:
				filter_type = FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_6DB;
				break;
			case FILTER_12DB:
				filter_type = FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_12DB;
				break;
			case FILTER_18DB:
				filter_type = FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_24DB;
				break;
			case FILTER_24DB:
				filter_type = FMOD_DSP_MULTIBAND_EQ_FILTER_LOWPASS_24DB;
				break;
		}

		// Q 值映射
		float q_value = 0.1f + resonance * 9.9f;

		// 更新参数
		eq_dsp->set_parameter_int(0, filter_type);
		eq_dsp->set_parameter_float(1, cutoff_hz);
		eq_dsp->set_parameter_float(2, q_value);
		eq_dsp->set_parameter_float(3, 20.0f * FmodUtils::linear_to_db(gain));
	}
}
