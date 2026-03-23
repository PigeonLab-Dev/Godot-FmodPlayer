#include "fmod_audio_effect_chorus.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"
#include <math.h>

namespace godot {
	void FmodAudioEffectChorus::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_voice_count", "voices"), &FmodAudioEffectChorus::set_voice_count);
		ClassDB::bind_method(D_METHOD("get_voice_count"), &FmodAudioEffectChorus::get_voice_count);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "voice_count", PROPERTY_HINT_RANGE, "1,4,1", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_UPDATE_ALL_IF_MODIFIED), "set_voice_count", "get_voice_count");

		ClassDB::bind_method(D_METHOD("set_wet", "amount"), &FmodAudioEffectChorus::set_wet);
		ClassDB::bind_method(D_METHOD("get_wet"), &FmodAudioEffectChorus::get_wet);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wet", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_wet", "get_wet");

		ClassDB::bind_method(D_METHOD("set_dry", "amount"), &FmodAudioEffectChorus::set_dry);
		ClassDB::bind_method(D_METHOD("get_dry"), &FmodAudioEffectChorus::get_dry);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dry", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_dry", "get_dry");

		ClassDB::bind_method(D_METHOD("set_voice_delay_ms", "voice_idx", "delay_ms"), &FmodAudioEffectChorus::set_voice_delay_ms);
		ClassDB::bind_method(D_METHOD("get_voice_delay_ms", "voice_idx"), &FmodAudioEffectChorus::get_voice_delay_ms);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/1/delay_ms", PROPERTY_HINT_RANGE, "0,50,0.01,suffix:ms"), "set_voice_delay_ms", "get_voice_delay_ms", 0);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/2/delay_ms", PROPERTY_HINT_RANGE, "0,50,0.01,suffix:ms"), "set_voice_delay_ms", "get_voice_delay_ms", 1);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/3/delay_ms", PROPERTY_HINT_RANGE, "0,50,0.01,suffix:ms"), "set_voice_delay_ms", "get_voice_delay_ms", 2);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/4/delay_ms", PROPERTY_HINT_RANGE, "0,50,0.01,suffix:ms"), "set_voice_delay_ms", "get_voice_delay_ms", 3);

		ClassDB::bind_method(D_METHOD("set_voice_rate_hz", "voice_idx", "rate_hz"), &FmodAudioEffectChorus::set_voice_rate_hz);
		ClassDB::bind_method(D_METHOD("get_voice_rate_hz", "voice_idx"), &FmodAudioEffectChorus::get_voice_rate_hz);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/1/rate_hz", PROPERTY_HINT_RANGE, "0.1,20,0.1,suffix:Hz"), "set_voice_rate_hz", "get_voice_rate_hz", 0);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/2/rate_hz", PROPERTY_HINT_RANGE, "0.1,20,0.1,suffix:Hz"), "set_voice_rate_hz", "get_voice_rate_hz", 1);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/3/rate_hz", PROPERTY_HINT_RANGE, "0.1,20,0.1,suffix:Hz"), "set_voice_rate_hz", "get_voice_rate_hz", 2);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/4/rate_hz", PROPERTY_HINT_RANGE, "0.1,20,0.1,suffix:Hz"), "set_voice_rate_hz", "get_voice_rate_hz", 3);

		ClassDB::bind_method(D_METHOD("set_voice_depth_ms", "voice_idx", "depth_ms"), &FmodAudioEffectChorus::set_voice_depth_ms);
		ClassDB::bind_method(D_METHOD("get_voice_depth_ms", "voice_idx"), &FmodAudioEffectChorus::get_voice_depth_ms);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/1/depth_ms", PROPERTY_HINT_RANGE, "0,20,0.01,suffix:ms"), "set_voice_depth_ms", "get_voice_depth_ms", 0);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/2/depth_ms", PROPERTY_HINT_RANGE, "0,20,0.01,suffix:ms"), "set_voice_depth_ms", "get_voice_depth_ms", 1);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/3/depth_ms", PROPERTY_HINT_RANGE, "0,20,0.01,suffix:ms"), "set_voice_depth_ms", "get_voice_depth_ms", 2);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/4/depth_ms", PROPERTY_HINT_RANGE, "0,20,0.01,suffix:ms"), "set_voice_depth_ms", "get_voice_depth_ms", 3);

		ClassDB::bind_method(D_METHOD("set_voice_level_db", "voice_idx", "level_db"), &FmodAudioEffectChorus::set_voice_level_db);
		ClassDB::bind_method(D_METHOD("get_voice_level_db", "voice_idx"), &FmodAudioEffectChorus::get_voice_level_db);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/1/level_db", PROPERTY_HINT_RANGE, "-60,24,0.1,suffix:dB"), "set_voice_level_db", "get_voice_level_db", 0);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/2/level_db", PROPERTY_HINT_RANGE, "-60,24,0.1,suffix:dB"), "set_voice_level_db", "get_voice_level_db", 1);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/3/level_db", PROPERTY_HINT_RANGE, "-60,24,0.1,suffix:dB"), "set_voice_level_db", "get_voice_level_db", 2);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/4/level_db", PROPERTY_HINT_RANGE, "-60,24,0.1,suffix:dB"), "set_voice_level_db", "get_voice_level_db", 3);

		ClassDB::bind_method(D_METHOD("set_voice_cutoff_hz", "voice_idx", "cutoff_hz"), &FmodAudioEffectChorus::set_voice_cutoff_hz);
		ClassDB::bind_method(D_METHOD("get_voice_cutoff_hz", "voice_idx"), &FmodAudioEffectChorus::get_voice_cutoff_hz);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/1/cutoff_hz", PROPERTY_HINT_RANGE, "1,20500,1,suffix:Hz"), "set_voice_cutoff_hz", "get_voice_cutoff_hz", 0);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/2/cutoff_hz", PROPERTY_HINT_RANGE, "1,20500,1,suffix:Hz"), "set_voice_cutoff_hz", "get_voice_cutoff_hz", 1);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/3/cutoff_hz", PROPERTY_HINT_RANGE, "1,20500,1,suffix:Hz"), "set_voice_cutoff_hz", "get_voice_cutoff_hz", 2);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/4/cutoff_hz", PROPERTY_HINT_RANGE, "1,20500,1,suffix:Hz"), "set_voice_cutoff_hz", "get_voice_cutoff_hz", 3);

		ClassDB::bind_method(D_METHOD("set_voice_pan", "voice_idx", "pan"), &FmodAudioEffectChorus::set_voice_pan);
		ClassDB::bind_method(D_METHOD("get_voice_pan", "voice_idx"), &FmodAudioEffectChorus::get_voice_pan);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/1/pan", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_voice_pan", "get_voice_pan", 0);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/2/pan", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_voice_pan", "get_voice_pan", 1);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/3/pan", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_voice_pan", "get_voice_pan", 2);
		ADD_PROPERTYI(PropertyInfo(Variant::FLOAT, "voice/4/pan", PROPERTY_HINT_RANGE, "-1,1,0.01"), "set_voice_pan", "get_voice_pan", 3);
	}

	void FmodAudioEffectChorus::_validate_property(PropertyInfo& p_property) const {
		if (p_property.name.begins_with("voice/")) {
			int voice_idx = p_property.name.get_slicec('/', 1).to_int();
			if (voice_idx > voice_count) {
				p_property.usage = PROPERTY_USAGE_NONE;
			}
		}
	}

	FmodAudioEffectChorus::FmodAudioEffectChorus() {
		voice_count = 2;
		voice[0].delay = 15;
		voice[1].delay = 20;
		voice[0].rate = 0.8;
		voice[1].rate = 1.2;
		voice[0].depth = 2;
		voice[1].depth = 3;
		voice[0].cutoff = MS_CUTOFF_MAX;  // 默认不启用低通滤波
		voice[1].cutoff = MS_CUTOFF_MAX;
		voice[0].pan = -0.5;
		voice[1].pan = 0.5;

		wet = 0.5;
		dry = 1.0;
	}

	FmodAudioEffectChorus::~FmodAudioEffectChorus() {
	}

	void FmodAudioEffectChorus::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		// 如果已经应用过，先移除
		if (bus.is_valid()) {
			remove_from_bus(bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		// 简化方案：使用单个 Chorus DSP，将多 voice 参数合并
		// 这样可以避免串联多个 DSP 导致的累积效应
		_create_merged_chorus_dsp(system);
	}

	void FmodAudioEffectChorus::_create_merged_chorus_dsp(Ref<FmodSystem> system) {
		// 使用 FMOD 内置 Chorus DSP
		Ref<FmodDSP> chorus_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_CHORUS);
		if (!chorus_dsp.is_valid()) {
			return;
		}

		// 计算所有 voice 的平均参数，模拟多 voice 效果
		float avg_rate = 0.0f;
		float avg_depth = 0.0f;
		float total_level_linear = 0.0f;
		
		for (int i = 0; i < voice_count; i++) {
			avg_rate += voice[i].rate;
			avg_depth += voice[i].depth;
			total_level_linear += FmodUtils::db_to_linear(voice[i].level);
		}
		avg_rate /= voice_count;
		avg_depth /= voice_count;
		
		// 计算平均音量（dB）
		float avg_level_db = FmodUtils::linear_to_db(total_level_linear / voice_count);

		// FMOD Chorus DSP 参数：
		// 0 = Mix (0.0 - 100.0) - 干信号比例！注意：这是原始信号的比例
		//     100 = 全干信号(无效果), 0 = 全湿信号(全效果), 50 = 各半
		// 1 = Rate (0.0 - 20.0 Hz) - 调制速率
		// 2 = Depth (0.0 - 100.0) - 调制深度百分比
		
		// Mix：根据 wet/dry 计算
		// wet=1, dry=0 -> Mix=0 (全湿)
		// wet=0, dry=1 -> Mix=100 (全干)
		// wet=0.5, dry=0.5 -> Mix=50 (各半)
		float total = wet + dry;
		if (total <= 0.0f) total = 0.001f; // 避免除零
		float mix_percent = (dry / total) * 100.0f;
		// 应用 voice level 调整
		float voice_level_factor = total_level_linear / voice_count;
		mix_percent = 100.0f - (wet * voice_level_factor * 100.0f);
		chorus_dsp->set_parameter_float(0, CLAMP(mix_percent, 0.0f, 100.0f));
		
		// Rate：使用平均速率
		chorus_dsp->set_parameter_float(1, CLAMP(avg_rate, 0.1f, 20.0f));
		
		// Depth：将 ms 平均值转换为百分比 (0-100)
		// FMOD Chorus 的 Depth 是百分比，不是归一化的 0-1
		float depth_percent = CLAMP(avg_depth * 5.0f, 0.0f, 100.0f);
		chorus_dsp->set_parameter_float(2, depth_percent);

		// 注意：FMOD Chorus DSP 内部已经处理了干湿混合
		// 通过 Mix 参数控制，不需要额外设置 set_wet_dry_mix
		// 但为了让效果更明显，我们让信号完全通过 Chorus
		chorus_dsp->set_wet_dry_mix(1.0f, 1.0f, 0.0f);

		// 添加到总线
		bus->add_dsp(-1, chorus_dsp);
		dsp_chain.push_back(chorus_dsp);

		// 只有在真正需要时才添加额外的 DSP
		// 检查是否有 voice 需要特殊的延迟、滤波或声像
		bool needs_delay = false;
		bool needs_filter = false;
		bool needs_pan = false;
		
		for (int i = 0; i < voice_count; i++) {
			if (voice[i].delay > 0.0f) needs_delay = true;
			if (voice[i].cutoff < MS_CUTOFF_MAX - 100) needs_filter = true;
			if (voice[i].pan != 0.0f) needs_pan = true;
		}

		// 如果大部分 voice 都有类似的延迟需求，添加一个整体的延迟
		if (needs_delay) {
			float avg_delay = 0.0f;
			for (int i = 0; i < voice_count; i++) {
				avg_delay += voice[i].delay;
			}
			avg_delay /= voice_count;
			
			if (avg_delay > 0.0f) {
				Ref<FmodDSP> delay_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_DELAY);
				if (delay_dsp.is_valid()) {
					// 设置通道 0 的延迟
					delay_dsp->set_parameter_float(0, avg_delay);
					
					// 湿/干混合 - 轻微效果
					// prewet=1.0(完全进入延迟), postwet=0.3(30%湿信号), dry=0.7(70%旁路)
					delay_dsp->set_wet_dry_mix(1.0f, 0.3f, 0.7f);
					
					bus->add_dsp(-1, delay_dsp);
					dsp_chain.push_back(delay_dsp);
				}
			}
		}

		// Pan 处理：FMOD Chorus 本身有立体声输出
		// 如果需要特殊的声像调整，可以使用 Pan DSP
		if (needs_pan) {
			// 计算平均声像位置
			float avg_pan = 0.0f;
			for (int i = 0; i < voice_count; i++) {
				avg_pan += voice[i].pan;
			}
			avg_pan /= voice_count;
			
			if (std::abs(avg_pan) > 0.1f) {
				Ref<FmodDSP> pan_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_PAN);
				if (pan_dsp.is_valid()) {
					// 参数 1 = Stereo Position (-100 到 100)
					float stereo_pos = avg_pan * 100.0f;
					pan_dsp->set_parameter_float(1, stereo_pos);
					
					bus->add_dsp(-1, pan_dsp);
					dsp_chain.push_back(pan_dsp);
				}
			}
		}
	}

	void FmodAudioEffectChorus::set_voice_count(int p_voices) {
		ERR_FAIL_COND(p_voices < 1 || p_voices > MAX_VOICES);
		voice_count = p_voices;
	}

	int FmodAudioEffectChorus::get_voice_count() const {
		return voice_count;
	}

	void FmodAudioEffectChorus::set_wet(float amount) {
		wet = CLAMP(amount, 0.0f, 1.0f);
	}

	float FmodAudioEffectChorus::get_wet() const {
		return wet;
	}

	void FmodAudioEffectChorus::set_dry(float amount) {
		dry = CLAMP(amount, 0.0f, 1.0f);
	}

	float FmodAudioEffectChorus::get_dry() const {
		return dry;
	}

	void FmodAudioEffectChorus::set_voice_delay_ms(int p_voice, float p_delay_ms) {
		ERR_FAIL_INDEX(p_voice, MAX_VOICES);
		voice[p_voice].delay = CLAMP(p_delay_ms, 0.0f, (float)MAX_DELAY_MS);
	}

	float FmodAudioEffectChorus::get_voice_delay_ms(int p_voice) const {
		ERR_FAIL_INDEX_V(p_voice, MAX_VOICES, 0);
		return voice[p_voice].delay;
	}

	void FmodAudioEffectChorus::set_voice_rate_hz(int p_voice, float p_rate_hz) {
		ERR_FAIL_INDEX(p_voice, MAX_VOICES);
		voice[p_voice].rate = CLAMP(p_rate_hz, 0.1f, 20.0f);
	}

	float FmodAudioEffectChorus::get_voice_rate_hz(int p_voice) const {
		ERR_FAIL_INDEX_V(p_voice, MAX_VOICES, 0);
		return voice[p_voice].rate;
	}

	void FmodAudioEffectChorus::set_voice_depth_ms(int p_voice, float p_depth_ms) {
		ERR_FAIL_INDEX(p_voice, MAX_VOICES);
		voice[p_voice].depth = CLAMP(p_depth_ms, 0.0f, (float)MAX_DEPTH_MS);
	}

	float FmodAudioEffectChorus::get_voice_depth_ms(int p_voice) const {
		ERR_FAIL_INDEX_V(p_voice, MAX_VOICES, 0);
		return voice[p_voice].depth;
	}

	void FmodAudioEffectChorus::set_voice_level_db(int p_voice, float p_level_db) {
		ERR_FAIL_INDEX(p_voice, MAX_VOICES);
		voice[p_voice].level = CLAMP(p_level_db, -60.0f, 24.0f);
	}

	float FmodAudioEffectChorus::get_voice_level_db(int p_voice) const {
		ERR_FAIL_INDEX_V(p_voice, MAX_VOICES, 0);
		return voice[p_voice].level;
	}

	void FmodAudioEffectChorus::set_voice_cutoff_hz(int p_voice, float p_cutoff_hz) {
		ERR_FAIL_INDEX(p_voice, MAX_VOICES);
		voice[p_voice].cutoff = CLAMP(p_cutoff_hz, 1.0f, (float)MS_CUTOFF_MAX);
	}

	float FmodAudioEffectChorus::get_voice_cutoff_hz(int p_voice) const {
		ERR_FAIL_INDEX_V(p_voice, MAX_VOICES, 0);
		return voice[p_voice].cutoff;
	}

	void FmodAudioEffectChorus::set_voice_pan(int p_voice, float p_pan) {
		ERR_FAIL_INDEX(p_voice, MAX_VOICES);
		voice[p_voice].pan = CLAMP(p_pan, -1.0f, 1.0f);
	}

	float FmodAudioEffectChorus::get_voice_pan(int p_voice) const {
		ERR_FAIL_INDEX_V(p_voice, MAX_VOICES, 0);
		return voice[p_voice].pan;
	}
}
