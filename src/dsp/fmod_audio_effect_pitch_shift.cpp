#include "dsp/fmod_audio_effect_pitch_shift.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"
#include <godot_cpp/core/class_db.hpp>
#include <cmath>

namespace godot {

	static float _fft_size_to_value(FmodAudioEffectPitchShift::FFTSize size) {
		switch (size) {
			case FmodAudioEffectPitchShift::FFT_SIZE_256: return 256.0f;
			case FmodAudioEffectPitchShift::FFT_SIZE_512: return 512.0f;
			case FmodAudioEffectPitchShift::FFT_SIZE_1024: return 1024.0f;
			case FmodAudioEffectPitchShift::FFT_SIZE_2048: return 2048.0f;
			case FmodAudioEffectPitchShift::FFT_SIZE_4096: return 4096.0f;
			default: return 1024.0f;
		}
	}

	void FmodAudioEffectPitchShift::_bind_methods() {
		BIND_ENUM_CONSTANT(FFT_SIZE_256);
		BIND_ENUM_CONSTANT(FFT_SIZE_512);
		BIND_ENUM_CONSTANT(FFT_SIZE_1024);
		BIND_ENUM_CONSTANT(FFT_SIZE_2048);
		BIND_ENUM_CONSTANT(FFT_SIZE_4096);
		BIND_ENUM_CONSTANT(FFT_SIZE_MAX);

		ClassDB::bind_method(D_METHOD("set_pitch_scale", "pitch_scale"), &FmodAudioEffectPitchShift::set_pitch_scale);
		ClassDB::bind_method(D_METHOD("get_pitch_scale"), &FmodAudioEffectPitchShift::get_pitch_scale);
		ClassDB::bind_method(D_METHOD("set_fft_size", "fft_size"), &FmodAudioEffectPitchShift::set_fft_size);
		ClassDB::bind_method(D_METHOD("get_fft_size"), &FmodAudioEffectPitchShift::get_fft_size);
		ClassDB::bind_method(D_METHOD("set_oversampling", "oversampling"), &FmodAudioEffectPitchShift::set_oversampling);
		ClassDB::bind_method(D_METHOD("get_oversampling"), &FmodAudioEffectPitchShift::get_oversampling);

		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch_scale", PROPERTY_HINT_RANGE, "0.0,16.0,0.01"), "set_pitch_scale", "get_pitch_scale");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "fft_size", PROPERTY_HINT_ENUM, "256,512,1024,2048,4096"), "set_fft_size", "get_fft_size");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "oversampling", PROPERTY_HINT_RANGE, "1,32,1"), "set_oversampling", "get_oversampling");
	}

	FmodAudioEffectPitchShift::FmodAudioEffectPitchShift() {

	}

	FmodAudioEffectPitchShift::~FmodAudioEffectPitchShift() {

	}

	void FmodAudioEffectPitchShift::apply_to(Ref<FmodChannelGroup> p_bus) {
		ERR_FAIL_COND_MSG(p_bus.is_null(), "Invalid bus");

		if (bus == p_bus && !dsp_chain.is_empty()) {
			remove_from_bus(p_bus);
		}

		bus = p_bus;

		Ref<FmodSystem> system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_MSG(system.is_null(), "FMOD system not initialized");

		Ref<FmodDSP> pitch_dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_PITCHSHIFT);
		if (!pitch_dsp.is_valid()) return;

		// FMOD PitchShift DSP 参数：
		// 0 = Pitch (float, 0.5 ~ 2.0)
		// 1 = FFT size (float, 256 ~ 4096)
		// 2 = Overlap (float, 1 ~ 32)
		// 3 = Max channels (float, 0 ~ 16)

		// Godot pitch_scale (0.0~16) 映射到 FMOD Pitch (0.5~2.0)
		// FMOD: 0.5 = -1八度, 1.0 = 原调, 2.0 = +1八度
		// 只支持 +/-1八度范围，超出部分截断
		float fmod_pitch = 1.0f;
		if (pitch_scale > 0.0f) {
			float octaves = std::log2(pitch_scale);
			fmod_pitch = std::pow(2.0f, CLAMP(octaves, -1.0f, 1.0f));
		}
		pitch_dsp->set_parameter_float(0, fmod_pitch);

		// FFT size
		pitch_dsp->set_parameter_float(1, _fft_size_to_value(fft_size));

		// Oversampling -> Overlap
		pitch_dsp->set_parameter_float(2, static_cast<float>(oversampling));

		// Max channels (默认 0)
		pitch_dsp->set_parameter_float(3, 0.0f);

		bus->add_dsp(-1, pitch_dsp);
		dsp_chain.push_back(pitch_dsp);
	}

	void FmodAudioEffectPitchShift::set_pitch_scale(float p_pitch_scale) {
		pitch_scale = CLAMP(p_pitch_scale, 0.0f, 16.0f);
	}

	float FmodAudioEffectPitchShift::get_pitch_scale() const {
		return pitch_scale;
	}

	void FmodAudioEffectPitchShift::set_fft_size(FFTSize p_fft_size) {
		if (p_fft_size >= FFT_SIZE_256 && p_fft_size < FFT_SIZE_MAX) {
			fft_size = p_fft_size;
		}
	}

	FmodAudioEffectPitchShift::FFTSize FmodAudioEffectPitchShift::get_fft_size() const {
		return fft_size;
	}

	void FmodAudioEffectPitchShift::set_oversampling(int p_oversampling) {
		oversampling = CLAMP(p_oversampling, 1, 32);
	}

	int FmodAudioEffectPitchShift::get_oversampling() const {
		return oversampling;
	}

}
