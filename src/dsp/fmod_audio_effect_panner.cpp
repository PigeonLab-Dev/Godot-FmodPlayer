#include "dsp/fmod_audio_effect_panner.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"
#include <godot_cpp/core/class_db.hpp>

namespace godot {

	void FmodAudioEffectPanner::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_pan", "pan"), &FmodAudioEffectPanner::set_pan);
		ClassDB::bind_method(D_METHOD("get_pan"), &FmodAudioEffectPanner::get_pan);

		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pan", PROPERTY_HINT_RANGE, "-1.0,1.0,0.01"), "set_pan", "get_pan");
	}

	FmodAudioEffectPanner::FmodAudioEffectPanner() {

	}

	FmodAudioEffectPanner::~FmodAudioEffectPanner() {

	}

	void FmodAudioEffectPanner::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		// 如果已经应用到了同一个 bus，先移除旧的
		if (bus == p_bus && !dsp_chain.is_empty()) {
			remove_from_bus(p_bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		// 创建 PAN DSP
		Ref<FmodDSP> pan_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_PAN);
		if (!pan_dsp.is_valid()) {
			return;
		}

		// FMOD PAN DSP 参数（2D 立体声相关）：
		// 0 = Panning Mode - 声像模式 (0=Mono, 1=Stereo, 2=Surround)
		// 1 = Stereo Position - 2D 立体声位置 (-100.0 = 左, 100.0 = 右)
		
		// 设置为立体声模式 (Stereo = 1)
		pan_dsp->set_parameter_int(0, 1);
		// 设置声像位置：Godot 使用 -1.0 ~ 1.0，FMOD 使用 -100.0 ~ 100.0
		pan_dsp->set_parameter_float(1, pan * 100.0f);

		bus->add_dsp(-1, pan_dsp);
		dsp_chain.push_back(pan_dsp);
	}

	void FmodAudioEffectPanner::set_pan(float p_pan) {
		pan = CLAMP(p_pan, -1.0f, 1.0f);
		
		// 如果已经应用到了 bus，实时更新参数
		// Godot 使用 -1.0 ~ 1.0，FMOD 使用 -100.0 ~ 100.0
		if (!dsp_chain.is_empty() && dsp_chain[0].is_valid()) {
			dsp_chain[0]->set_parameter_float(1, pan * 100.0f);
		}
	}

	float FmodAudioEffectPanner::get_pan() const {
		return pan;
	}

} // namespace godot
