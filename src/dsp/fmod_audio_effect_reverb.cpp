#include "dsp/fmod_audio_effect_reverb.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {

	void FmodAudioEffectReverb::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_damping", "damping"), &FmodAudioEffectReverb::set_damping);
		ClassDB::bind_method(D_METHOD("get_damping"), &FmodAudioEffectReverb::get_damping);
		ClassDB::bind_method(D_METHOD("set_dry", "dry"), &FmodAudioEffectReverb::set_dry);
		ClassDB::bind_method(D_METHOD("get_dry"), &FmodAudioEffectReverb::get_dry);
		ClassDB::bind_method(D_METHOD("set_hpf", "hpf"), &FmodAudioEffectReverb::set_hpf);
		ClassDB::bind_method(D_METHOD("get_hpf"), &FmodAudioEffectReverb::get_hpf);
		ClassDB::bind_method(D_METHOD("set_predelay_feedback", "predelay_feedback"), &FmodAudioEffectReverb::set_predelay_feedback);
		ClassDB::bind_method(D_METHOD("get_predelay_feedback"), &FmodAudioEffectReverb::get_predelay_feedback);
		ClassDB::bind_method(D_METHOD("set_predelay_msec", "predelay_msec"), &FmodAudioEffectReverb::set_predelay_msec);
		ClassDB::bind_method(D_METHOD("get_predelay_msec"), &FmodAudioEffectReverb::get_predelay_msec);
		ClassDB::bind_method(D_METHOD("set_room_size", "room_size"), &FmodAudioEffectReverb::set_room_size);
		ClassDB::bind_method(D_METHOD("get_room_size"), &FmodAudioEffectReverb::get_room_size);
		ClassDB::bind_method(D_METHOD("set_spread", "spread"), &FmodAudioEffectReverb::set_spread);
		ClassDB::bind_method(D_METHOD("get_spread"), &FmodAudioEffectReverb::get_spread);
		ClassDB::bind_method(D_METHOD("set_wet", "wet"), &FmodAudioEffectReverb::set_wet);
		ClassDB::bind_method(D_METHOD("get_wet"), &FmodAudioEffectReverb::get_wet);

		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "damping", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_damping", "get_damping");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dry", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_dry", "get_dry");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "hipass", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_hpf", "get_hpf");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "predelay_feedback", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_predelay_feedback", "get_predelay_feedback");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "predelay_msec", PROPERTY_HINT_RANGE, "0,500,0.1,suffix:ms"), "set_predelay_msec", "get_predelay_msec");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "room_size", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_room_size", "get_room_size");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "spread", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_spread", "get_spread");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wet", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_wet", "get_wet");
	}

	FmodAudioEffectReverb::FmodAudioEffectReverb() {
	}

	FmodAudioEffectReverb::~FmodAudioEffectReverb() {
	}

	void FmodAudioEffectReverb::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		if (bus == p_bus && !dsp_chain.is_empty()) {
			remove_from_bus(p_bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		Ref<FmodDSP> reverb_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_SFXREVERB);
		if (!reverb_dsp.is_valid()) return;

		// FMOD SFXREVERB DSP 参数：
		// 0 = Decay Time (ms) - 100 ~ 20000, default 1500
		// 1 = Early Delay (ms) - 0 ~ 300, default 20
		// 2 = Late Delay (ms) - 0 ~ 100, default 40
		// 3 = HF Reference (Hz) - 20 ~ 20000, default 5000
		// 4 = HF Decay Ratio (%) - 10 ~ 200, default 50
		// 5 = Diffusion (%) - 0 ~ 100, default 100
		// 6 = Density (%) - 0 ~ 100, default 100
		// 7 = Low Shelf Frequency (Hz) - 20 ~ 1000, default 250
		// 8 = Low Shelf Gain (dB) - -48 ~ 12, default 0
		// 9 = High Cut (Hz) - 20 ~ 20000, default 20000
		// 10 = Early/Late Mix (%) - 0 ~ 100, default 50
		// 11 = Wet Level (dB) - -80 ~ 20, default -6
		// 12 = Dry Level (dB) - -80 ~ 20, default 0

		// 参数映射：
		// room_size (0-1) -> Decay Time (100-20000ms)
		float decay_time = 100.0f + room_size * 19900.0f;
		reverb_dsp->set_parameter_float(0, decay_time);

		// predelay_msec -> Early Delay (混响预延迟)
		reverb_dsp->set_parameter_float(1, CLAMP(predelay_msec, 0.0f, 300.0f));

		// Late Delay: 使用较小的固定值，避免产生明显的双重延迟效果
		// Godot 的 predelay_feedback 更像是预延迟的反馈强度，不是延迟时间
		// 这里将其映射到 Early/Late Mix，控制早期和晚期反射的混合比例
		reverb_dsp->set_parameter_float(2, 0.0f); // Late Delay 设为 0，避免额外的延迟

		// hipass (0-1) -> HF Reference (20-20000Hz, 反比)
		// hipass 越高，HF Reference 越低
		float hf_reference = 20000.0f - hipass * 19980.0f;
		reverb_dsp->set_parameter_float(3, hf_reference);

		// damping (0-1) -> HF Decay Ratio (10-200%)
		// damping 越高，高频衰减越快，所以 HF Decay Ratio 越低
		float hf_decay = 10.0f + (1.0f - damping) * 190.0f;
		reverb_dsp->set_parameter_float(4, hf_decay);

		// spread (0-1) -> Diffusion (0-100%)
		reverb_dsp->set_parameter_float(5, spread * 100.0f);

		// room_size 也影响 Density
		reverb_dsp->set_parameter_float(6, room_size * 100.0f);

		// Low Shelf Frequency 保持默认 250Hz
		reverb_dsp->set_parameter_float(7, 250.0f);

		// Low Shelf Gain 保持默认 0dB
		reverb_dsp->set_parameter_float(8, 0.0f);

		// hipass 也影响 High Cut
		float high_cut = 20000.0f - hipass * 15000.0f;
		reverb_dsp->set_parameter_float(9, high_cut);

		// Early/Late Mix: 使用固定值 50% (FMOD默认)，让早期和晚期反射均衡
		// predelay_feedback 在 Godot 中是预延迟反馈，在 FMOD 中没有直接对应
		reverb_dsp->set_parameter_float(10, 50.0f);

		// wet (0-1) -> Wet Level (-80 ~ 20 dB)
		// 0 -> -80dB (静音), 0.5 -> -6dB (FMOD默认), 1 -> 0dB (全音量)
		float wet_db = (wet <= 0.0f) ? -80.0f : (-12.0f + wet * 12.0f);
		reverb_dsp->set_parameter_float(11, wet_db);

		// dry (0-1) -> Dry Level (-80 ~ 20 dB)
		// 0 -> -80dB (静音), 1 -> 0dB (全音量)
		float dry_db = (dry <= 0.0f) ? -80.0f : (-80.0f + dry * 80.0f);
		reverb_dsp->set_parameter_float(12, dry_db);

		bus->add_dsp(-1, reverb_dsp);
		dsp_chain.push_back(reverb_dsp);
	}

	void FmodAudioEffectReverb::set_damping(float p_damping) {
		damping = CLAMP(p_damping, 0.0f, 1.0f);
	}

	float FmodAudioEffectReverb::get_damping() const {
		return damping;
	}

	void FmodAudioEffectReverb::set_dry(float p_dry) {
		dry = CLAMP(p_dry, 0.0f, 1.0f);
	}

	float FmodAudioEffectReverb::get_dry() const {
		return dry;
	}

	void FmodAudioEffectReverb::set_hpf(float p_hpf) {
		hipass = CLAMP(p_hpf, 0.0f, 1.0f);
	}

	float FmodAudioEffectReverb::get_hpf() const {
		return hipass;
	}

	void FmodAudioEffectReverb::set_predelay_feedback(float p_predelay_feedback) {
		predelay_feedback = CLAMP(p_predelay_feedback, 0.0f, 1.0f);
	}

	float FmodAudioEffectReverb::get_predelay_feedback() const {
		return predelay_feedback;
	}

	void FmodAudioEffectReverb::set_predelay_msec(float p_predelay_msec) {
		predelay_msec = CLAMP(p_predelay_msec, 0.0f, 500.0f);
	}

	float FmodAudioEffectReverb::get_predelay_msec() const {
		return predelay_msec;
	}

	void FmodAudioEffectReverb::set_room_size(float p_room_size) {
		room_size = CLAMP(p_room_size, 0.0f, 1.0f);
	}

	float FmodAudioEffectReverb::get_room_size() const {
		return room_size;
	}

	void FmodAudioEffectReverb::set_spread(float p_spread) {
		spread = CLAMP(p_spread, 0.0f, 1.0f);
	}

	float FmodAudioEffectReverb::get_spread() const {
		return spread;
	}

	void FmodAudioEffectReverb::set_wet(float p_wet) {
		wet = CLAMP(p_wet, 0.0f, 1.0f);
	}

	float FmodAudioEffectReverb::get_wet() const {
		return wet;
	}

}
