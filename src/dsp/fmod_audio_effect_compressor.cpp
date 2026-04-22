#include "fmod_audio_effect_compressor.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/audio_server.hpp>

namespace godot {
	void FmodAudioEffectCompressor::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_threshold", "threshold_db"), &FmodAudioEffectCompressor::set_threshold);
		ClassDB::bind_method(D_METHOD("get_threshold"), &FmodAudioEffectCompressor::get_threshold);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "threshold_db", PROPERTY_HINT_RANGE, "-60,0,0.1,suffix:dB"), "set_threshold", "get_threshold");
		
		ClassDB::bind_method(D_METHOD("set_ratio", "ratio"), &FmodAudioEffectCompressor::set_ratio);
		ClassDB::bind_method(D_METHOD("get_ratio"), &FmodAudioEffectCompressor::get_ratio);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ratio", PROPERTY_HINT_RANGE, "1,48,0.1"), "set_ratio", "get_ratio");
		
		ClassDB::bind_method(D_METHOD("set_gain", "gain_db"), &FmodAudioEffectCompressor::set_gain);
		ClassDB::bind_method(D_METHOD("get_gain"), &FmodAudioEffectCompressor::get_gain);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gain_db", PROPERTY_HINT_RANGE, "-20,20,0.1,suffix:dB"), "set_gain", "get_gain");
		
		ClassDB::bind_method(D_METHOD("set_attack_us", "attack_us"), &FmodAudioEffectCompressor::set_attack_us);
		ClassDB::bind_method(D_METHOD("get_attack_us"), &FmodAudioEffectCompressor::get_attack_us);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_us", PROPERTY_HINT_RANGE, U"20,200000,1,suffix:µs"), "set_attack_us", "get_attack_us");
		
		ClassDB::bind_method(D_METHOD("set_release_ms", "release_ms"), &FmodAudioEffectCompressor::set_release_ms);
		ClassDB::bind_method(D_METHOD("get_release_ms"), &FmodAudioEffectCompressor::get_release_ms);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "release_ms", PROPERTY_HINT_RANGE, "20,5000,1,suffix:ms"), "set_release_ms", "get_release_ms");
		
		ClassDB::bind_method(D_METHOD("set_mix", "mix"), &FmodAudioEffectCompressor::set_mix);
		ClassDB::bind_method(D_METHOD("get_mix"), &FmodAudioEffectCompressor::get_mix);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mix", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_mix", "get_mix");
		
		ClassDB::bind_method(D_METHOD("set_sidechain", "sidechain"), &FmodAudioEffectCompressor::set_sidechain);
		ClassDB::bind_method(D_METHOD("get_sidechain"), &FmodAudioEffectCompressor::get_sidechain);
		ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "sidechain", PROPERTY_HINT_ENUM), "set_sidechain", "get_sidechain");
	}

	void FmodAudioEffectCompressor::_validate_property(PropertyInfo& p_property) const {
		if (!Engine::get_singleton()->is_editor_hint()) {
			return;
		}
		if (p_property.name == StringName("sidechain")) {
			String buses = "";
			for (int i = 0; i < AudioServer::get_singleton()->get_bus_count(); i++) {
				if (i > 0) buses += ",";
				buses += AudioServer::get_singleton()->get_bus_name(i);
			}

			p_property.hint_string = buses;
		}
	}

	FmodAudioEffectCompressor::FmodAudioEffectCompressor() {
		threshold = 0.0f;           // dB, 默认 0 (无压缩)
		ratio = 4.0f;               // 默认压缩比 4:1
		gain = 0.0f;                // dB, 默认无增益补偿
		attack_us = 20000.0f;       // 20ms = 20000us
		release_ms = 250.0f;        // 250ms
		mix = 1.0f;                 // 全湿信号（完全压缩）
		sidechain = StringName();   // 无旁链
	}

	FmodAudioEffectCompressor::~FmodAudioEffectCompressor() {
	}

	void FmodAudioEffectCompressor::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		// 如果已经应用过，先移除
		if (bus.is_valid()) {
			remove_from_bus(bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		// 创建 FMOD Compressor DSP
		Ref<FmodDSP> compressor_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_COMPRESSOR);
		ERR_FAIL_COND_MSG(!compressor_dsp.is_valid(), "Failed to create compressor DSP");

		// FMOD Compressor DSP 参数映射：
		// 0 = Threshold [-60, 0] dB
		// 1 = Ratio [1, 50] 
		// 2 = Attack [0.1, 500] ms
		// 3 = Release [10, 5000] ms
		// 4 = Make up gain [-30, 30] dB
		// 5 = Use sidechain (data parameter)
		// 6 = Linked [true/false]

		// Threshold: Godot [-60, 0] -> FMOD [-60, 0] (直接映射)
		float fmod_threshold = CLAMP(threshold, -60.0f, 0.0f);
		compressor_dsp->set_parameter_float(0, fmod_threshold);

		// Ratio: Godot [1, 48] -> FMOD [1, 50] (直接映射)
		float fmod_ratio = CLAMP(ratio, 1.0f, 50.0f);
		compressor_dsp->set_parameter_float(1, fmod_ratio);

		// Attack: Godot 微秒 -> FMOD 毫秒 (转换)
		// Godot: 20us - 200000us (0.2s), FMOD: 0.1ms - 500ms
		float attack_ms = attack_us / 1000.0f;
		float fmod_attack = CLAMP(attack_ms, 0.1f, 500.0f);
		compressor_dsp->set_parameter_float(2, fmod_attack);

		// Release: Godot [20, 5000] ms -> FMOD [10, 5000] ms (直接映射)
		float fmod_release = CLAMP(release_ms, 10.0f, 5000.0f);
		compressor_dsp->set_parameter_float(3, fmod_release);

		// Make up gain: Godot [-20, 20] dB -> FMOD [-30, 30] dB (直接映射)
		float fmod_gain = CLAMP(gain, -30.0f, 30.0f);
		compressor_dsp->set_parameter_float(4, fmod_gain);

		// Linked: 默认 true (多声道联动)
		compressor_dsp->set_parameter_bool(6, true);

		// 添加到总线
		bus->add_dsp(-1, compressor_dsp);
		dsp_chain.push_back(compressor_dsp);

		// 处理 Mix 参数：FMOD Compressor 没有内置 mix，使用 wet/dry mix
		// mix = 1.0: 全压缩信号, mix = 0.0: 全原信号
		// prewet: 进入处理的信号比例
		// postwet: 处理后输出比例  
		// dry: 旁路输出比例
		float dry_amount = 1.0f - mix;
		float wet_amount = mix;
		
		// 让信号完全进入 compressor 处理，然后通过 wet/dry 控制输出比例
		compressor_dsp->set_wet_dry_mix(1.0f, wet_amount, dry_amount);

		// 处理旁链 (sidechain) - 如果需要的话
		// FMOD 旁链实现较复杂，需要 SEND/RETURN DSP 配合
		// 这里简单处理：如果指定了 sidechain，打印警告
		if (sidechain != StringName()) {
			// TODO: 实现旁链支持
			// 需要创建 SEND DSP 将信号发送到旁链总线
			// 然后设置 compressor 的 sidechain 数据源
			WARN_PRINT("Sidechain compression not yet fully implemented in FMOD");
		}
	}

	void FmodAudioEffectCompressor::set_threshold(float p_threshold) {
		threshold = CLAMP(p_threshold, -60.0f, 0.0f);
	}

	float FmodAudioEffectCompressor::get_threshold() const {
		return threshold;
	}

	void FmodAudioEffectCompressor::set_ratio(float p_ratio) {
		ratio = CLAMP(p_ratio, 1.0f, 48.0f);
	}

	float FmodAudioEffectCompressor::get_ratio() const {
		return ratio;
	}

	void FmodAudioEffectCompressor::set_gain(float p_gain) {
		gain = CLAMP(p_gain, -20.0f, 20.0f);
	}

	float FmodAudioEffectCompressor::get_gain() const {
		return gain;
	}

	void FmodAudioEffectCompressor::set_attack_us(float p_attack_us) {
		attack_us = CLAMP(p_attack_us, 20.0f, 200000.0f);
	}

	float FmodAudioEffectCompressor::get_attack_us() const {
		return attack_us;
	}

	void FmodAudioEffectCompressor::set_release_ms(float p_release_ms) {
		release_ms = CLAMP(p_release_ms, 20.0f, 5000.0f);
	}

	float FmodAudioEffectCompressor::get_release_ms() const {
		return release_ms;
	}

	void FmodAudioEffectCompressor::set_mix(float p_mix) {
		mix = CLAMP(p_mix, 0.0f, 1.0f);
	}

	float FmodAudioEffectCompressor::get_mix() const {
		return mix;
	}

	void FmodAudioEffectCompressor::set_sidechain(const StringName& p_sidechain) {
		sidechain = p_sidechain;
	}

	StringName FmodAudioEffectCompressor::get_sidechain() const {
		return sidechain;
	}
}
