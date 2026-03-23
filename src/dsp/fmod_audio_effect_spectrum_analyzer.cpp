#include "dsp/fmod_audio_effect_spectrum_analyzer.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"
#include <godot_cpp/core/class_db.hpp>
#include <cmath>

namespace godot {

	static int _fft_size_to_value(FmodAudioEffectSpectrumAnalyzer::FFTSize size) {
		switch (size) {
			case FmodAudioEffectSpectrumAnalyzer::FFT_SIZE_256: return 256;
			case FmodAudioEffectSpectrumAnalyzer::FFT_SIZE_512: return 512;
			case FmodAudioEffectSpectrumAnalyzer::FFT_SIZE_1024: return 1024;
			case FmodAudioEffectSpectrumAnalyzer::FFT_SIZE_2048: return 2048;
			case FmodAudioEffectSpectrumAnalyzer::FFT_SIZE_4096: return 4096;
			default: return 1024;
		}
	}

	void FmodAudioEffectSpectrumAnalyzer::_bind_methods() {
		BIND_ENUM_CONSTANT(FFT_SIZE_256);
		BIND_ENUM_CONSTANT(FFT_SIZE_512);
		BIND_ENUM_CONSTANT(FFT_SIZE_1024);
		BIND_ENUM_CONSTANT(FFT_SIZE_2048);
		BIND_ENUM_CONSTANT(FFT_SIZE_4096);
		BIND_ENUM_CONSTANT(FFT_SIZE_MAX);

		BIND_ENUM_CONSTANT(MAGNITUDE_AVERAGE);
		BIND_ENUM_CONSTANT(MAGNITUDE_MAX);

		ClassDB::bind_method(D_METHOD("set_buffer_length", "buffer_length"), &FmodAudioEffectSpectrumAnalyzer::set_buffer_length);
		ClassDB::bind_method(D_METHOD("get_buffer_length"), &FmodAudioEffectSpectrumAnalyzer::get_buffer_length);
		ClassDB::bind_method(D_METHOD("set_fft_size", "fft_size"), &FmodAudioEffectSpectrumAnalyzer::set_fft_size);
		ClassDB::bind_method(D_METHOD("get_fft_size"), &FmodAudioEffectSpectrumAnalyzer::get_fft_size);
		ClassDB::bind_method(D_METHOD("get_magnitude_for_frequency_range", "begin", "end", "mode"), &FmodAudioEffectSpectrumAnalyzer::get_magnitude_for_frequency_range, DEFVAL(MAGNITUDE_MAX));

		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "buffer_length", PROPERTY_HINT_RANGE, "0.1,10,0.1,suffix:s"), "set_buffer_length", "get_buffer_length");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "fft_size", PROPERTY_HINT_ENUM, "256,512,1024,2048,4096"), "set_fft_size", "get_fft_size");
	}

	FmodAudioEffectSpectrumAnalyzer::FmodAudioEffectSpectrumAnalyzer() {
	}

	FmodAudioEffectSpectrumAnalyzer::~FmodAudioEffectSpectrumAnalyzer() {
	}

	void FmodAudioEffectSpectrumAnalyzer::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		if (bus == p_bus && !dsp_chain.is_empty()) {
			remove_from_bus(p_bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		// 创建 FFT DSP
		Ref<FmodDSP> fft_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_FFT);
		if (!fft_dsp.is_valid()) return;

		// FMOD FFT DSP 参数：
		// 0 = Size (int) - FFT 窗口大小: 128, 256, 512, 1024, 2048, 4096, 8192, 16384
		// 1 = Type (int) - 窗口形状: 0=rect, 1=tri, 2=hamming, 3=hanning, 4=blackman, 5=blackman harris
		// 2 = Band Start Freq (float) - 分析频段起始频率
		// 3 = Band Stop Freq (float) - 分析频段截止频率
		// 4 = Spectrum Data (data) - 频谱数据
		// 5 = RMS (float) - 分析频段内的信号 RMS
		// 6 = Centroid (float) - 频谱质心
		// 7 = Immediate Mode (bool) - 立即渲染模式
		// 8 = Downmix mode (int) - 0=不混音, 1=混音为单声道
		// 9 = Channel (int) - 要分析的通道，-1=全部

		// 设置 FFT 大小
		fft_dsp->set_parameter_int(0, _fft_size_to_value(fft_size));

		// 设置窗口类型为 Hanning (默认)
		fft_dsp->set_parameter_int(1, 3);

		// 分析全频段
		fft_dsp->set_parameter_float(2, 0.0f);
		fft_dsp->set_parameter_float(3, 22000.0f);

		// 混音为单声道以获得更稳定的分析结果
		fft_dsp->set_parameter_int(8, 1);

		bus->add_dsp(-1, fft_dsp);
		dsp_chain.push_back(fft_dsp);
	}

	void FmodAudioEffectSpectrumAnalyzer::set_buffer_length(float p_buffer_length) {
		buffer_length = CLAMP(p_buffer_length, 0.1f, 10.0f);
	}

	float FmodAudioEffectSpectrumAnalyzer::get_buffer_length() const {
		return buffer_length;
	}

	void FmodAudioEffectSpectrumAnalyzer::set_fft_size(FFTSize p_fft_size) {
		if (p_fft_size >= FFT_SIZE_256 && p_fft_size < FFT_SIZE_MAX) {
			fft_size = p_fft_size;
		}
	}

	FmodAudioEffectSpectrumAnalyzer::FFTSize FmodAudioEffectSpectrumAnalyzer::get_fft_size() const {
		return fft_size;
	}

	Vector2 FmodAudioEffectSpectrumAnalyzer::get_magnitude_for_frequency_range(float p_begin, float p_end, MagnitudeMode p_mode) const {
		if (spectrum_data.is_empty() || p_begin >= p_end) {
			return Vector2();
		}

		// 简单的线性插值查找频率对应的幅度
		// 假设频谱数据是均匀分布的，从 0 到 sample_rate/2
		// 这里使用简化的实现

		float sample_rate = 48000.0f; // 假设默认采样率
		float nyquist = sample_rate / 2.0f;
		int num_bins = spectrum_data.size();

		if (num_bins <= 0) {
			return Vector2();
		}

		// 计算频率对应的 bin 索引
		int begin_bin = static_cast<int>((p_begin / nyquist) * num_bins);
		int end_bin = static_cast<int>((p_end / nyquist) * num_bins);

		begin_bin = CLAMP(begin_bin, 0, num_bins - 1);
		end_bin = CLAMP(end_bin, 0, num_bins - 1);

		if (begin_bin >= end_bin) {
			end_bin = begin_bin + 1;
		}

		float max_magnitude = 0.0f;
		float avg_magnitude = 0.0f;

		for (int i = begin_bin; i < end_bin && i < num_bins; i++) {
			float mag = spectrum_data[i].length();
			max_magnitude = MAX(max_magnitude, mag);
			avg_magnitude += mag;
		}

		avg_magnitude /= (end_bin - begin_bin);

		if (p_mode == MAGNITUDE_MAX) {
			return Vector2(max_magnitude, max_magnitude);
		} else {
			return Vector2(avg_magnitude, avg_magnitude);
		}
	}

	void FmodAudioEffectSpectrumAnalyzer::_update_spectrum_data(const float* spectrum, int num_bins, float sample_rate) {
		spectrum_data.resize(num_bins);
		for (int i = 0; i < num_bins; i++) {
			// FMOD FFT 数据通常是幅度值
			spectrum_data.write[i] = Vector2(spectrum[i], spectrum[i]);
		}
	}

	void FmodAudioEffectSpectrumAnalyzer::_update_rms(float rms) {
		current_rms = rms;
	}

	void FmodAudioEffectSpectrumAnalyzer::_update_centroid(float centroid) {
		current_centroid = centroid;
	}

}
