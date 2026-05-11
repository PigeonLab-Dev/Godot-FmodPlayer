#ifndef FMOD_AUDIO_EFFECT_EQ_H
#define FMOD_AUDIO_EFFECT_EQ_H

#include "dsp/fmod_audio_effect.h"
#include <godot_cpp/templates/vector.hpp>

namespace godot {
	class FmodAudioEffectEQ : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectEQ, FmodAudioEffect)

	public:
		enum Preset {
			PRESET_6_BANDS = 0,     										// 6 频段均衡器 (32Hz - 10000Hz)
			PRESET_10_BANDS = 1,    										// 10 频段均衡器 (31Hz - 16000Hz)
			PRESET_21_BANDS = 2,    										// 21 频段均衡器 (22Hz - 22000Hz)
		};

		enum FilterType {
			FILTER_DISABLED = 0,
			FILTER_LOWPASS_12DB,
			FILTER_LOWPASS_24DB,
			FILTER_LOWPASS_48DB,
			FILTER_HIGHPASS_12DB,
			FILTER_HIGHPASS_24DB,
			FILTER_HIGHPASS_48DB,
			FILTER_LOWSHELF,        										// 低频搁架 - 用于 EQ 增益调节
			FILTER_HIGHSHELF,      											// 高频搁架 - 用于 EQ 增益调节
			FILTER_PEAKING,         										// 峰值滤波 - 用于 EQ 增益调节（默认）
			FILTER_BANDPASS,
			FILTER_NOTCH,
			FILTER_ALLPASS,
			FILTER_LOWPASS_6DB,
			FILTER_HIGHPASS_6DB,
		};

		// 频段信息结构
		struct BandInfo {
			float frequency = 1000.0f;      								// 中心频率 (Hz)
			float gain_db = 0.0f;           								// 增益 (dB), 范围 -60 ~ +24
			FilterType filter_type = FILTER_PEAKING;  						// 默认使用峰值滤波器
			float q = 1.0f;                 								// Q值 (带宽)，默认 1.0
		};

	protected:
		static void _bind_methods();

	private:
		Preset preset = PRESET_6_BANDS;    									// EQ 预设
		Vector<BandInfo> bands;             								// 频段配置
		Vector<String> band_names;          								// 频段名称

		void _init_bands();													// 初始化频段配置
		void _get_preset_frequencies(
			Preset p_preset,
			Vector<float>& r_frequencies
		) const;															// 根据预设获取默认频段频率
		int _get_required_dsp_count() const;								// 计算需要多少个 MULTIBAND_EQ DSP (每个支持 5 个频段)
		void _get_dsp_band_indices(
			int p_band_idx,
			int& r_dsp_idx,
			int& r_band_letter_idx
		) const;															// 获取指定频段对应的 DSP 索引和频段字母索引 (0-4 对应 A-E)
		void _update_band_dsp(int p_band_idx);								// 更新指定频段的 DSP 参数

	public:
		FmodAudioEffectEQ();
		explicit FmodAudioEffectEQ(Preset p_preset);
		virtual ~FmodAudioEffectEQ();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		// 预设
		void set_preset(Preset p_preset);
		Preset get_preset() const;

		// 获取频段数量
		int get_band_count() const;

		// 指定频段的增益 (dB, 范围 -60 ~ +24)
		void set_band_gain_db(int p_band_idx, float p_gain_db);
		float get_band_gain_db(int p_band_idx) const;

		// 指定频段的频率
		void set_band_frequency(int p_band_idx, float p_frequency);
		float get_band_frequency(int p_band_idx) const;

		// 指定频段的滤波器类型
		void set_band_filter_type(int p_band_idx, FilterType p_type);
		FilterType get_band_filter_type(int p_band_idx) const;

		// 指定频段的 Q 值
		void set_band_q(int p_band_idx, float p_q);
		float get_band_q(int p_band_idx) const;

		// 便捷的属性访问方法（用于 Godot 编辑器）
		// 格式: band_0_gain, band_0_frequency, band_0_q, band_0_filter_type 等
		bool _set(const StringName& p_name, const Variant& p_value);
		bool _get(const StringName& p_name, Variant& r_ret) const;
		void _get_property_list(List<PropertyInfo>* p_list) const;
	};

	class FmodAudioEffectEQ6 : public FmodAudioEffectEQ {
		GDCLASS(FmodAudioEffectEQ6, FmodAudioEffectEQ)
	protected:
		static void _bind_methods() {}
	public:
		FmodAudioEffectEQ6() : FmodAudioEffectEQ(PRESET_6_BANDS) {}
	};

	class FmodAudioEffectEQ10 : public FmodAudioEffectEQ {
		GDCLASS(FmodAudioEffectEQ10, FmodAudioEffectEQ)
	protected:
		static void _bind_methods() {}
	public:
		FmodAudioEffectEQ10() : FmodAudioEffectEQ(PRESET_10_BANDS) {}
	};

	class FmodAudioEffectEQ21 : public FmodAudioEffectEQ {
		GDCLASS(FmodAudioEffectEQ21, FmodAudioEffectEQ)
	protected:
		static void _bind_methods() {}
	public:
		FmodAudioEffectEQ21() : FmodAudioEffectEQ(PRESET_21_BANDS) {}
	};
}

VARIANT_ENUM_CAST(FmodAudioEffectEQ::Preset);
VARIANT_ENUM_CAST(FmodAudioEffectEQ::FilterType);

#endif // !FMOD_AUDIO_EFFECT_EQ_H
