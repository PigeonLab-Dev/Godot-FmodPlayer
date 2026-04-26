#include "audio/fmod_audio_stream_flac.h"

namespace godot {
	void FmodAudioStreamFLAC::_bind_methods() {
	}

	FmodAudioStreamFLAC::FmodAudioStreamFLAC() {
		create_mode_flags = MODE_STREAM;
	}

	FmodAudioStreamFLAC::~FmodAudioStreamFLAC() {
	}
}
