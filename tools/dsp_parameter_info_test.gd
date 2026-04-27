extends SceneTree

const DSP_TYPES := [
	[FmodDSP.DSP_TYPE_UNKNOWN, "UNKNOWN"],
	[FmodDSP.DSP_TYPE_MIXER, "MIXER"],
	[FmodDSP.DSP_TYPE_OSCILLATOR, "OSCILLATOR"],
	[FmodDSP.DSP_TYPE_LOWPASS, "LOWPASS"],
	[FmodDSP.DSP_TYPE_ITLOWPASS, "ITLOWPASS"],
	[FmodDSP.DSP_TYPE_HIGHPASS, "HIGHPASS"],
	[FmodDSP.DSP_TYPE_ECHO, "ECHO"],
	[FmodDSP.DSP_TYPE_FADER, "FADER"],
	[FmodDSP.DSP_TYPE_FLANGE, "FLANGE"],
	[FmodDSP.DSP_TYPE_DISTORTION, "DISTORTION"],
	[FmodDSP.DSP_TYPE_NORMALIZE, "NORMALIZE"],
	[FmodDSP.DSP_TYPE_LIMITER, "LIMITER"],
	[FmodDSP.DSP_TYPE_PARAMEQ, "PARAMEQ"],
	[FmodDSP.DSP_TYPE_PITCHSHIFT, "PITCHSHIFT"],
	[FmodDSP.DSP_TYPE_CHORUS, "CHORUS"],
	[FmodDSP.DSP_TYPE_ITECHO, "ITECHO"],
	[FmodDSP.DSP_TYPE_COMPRESSOR, "COMPRESSOR"],
	[FmodDSP.DSP_TYPE_SFXREVERB, "SFXREVERB"],
	[FmodDSP.DSP_TYPE_LOWPASS_SIMPLE, "LOWPASS_SIMPLE"],
	[FmodDSP.DSP_TYPE_DELAY, "DELAY"],
	[FmodDSP.DSP_TYPE_TREMOLO, "TREMOLO"],
	[FmodDSP.DSP_TYPE_SEND, "SEND"],
	[FmodDSP.DSP_TYPE_RETURN, "RETURN"],
	[FmodDSP.DSP_TYPE_HIGHPASS_SIMPLE, "HIGHPASS_SIMPLE"],
	[FmodDSP.DSP_TYPE_PAN, "PAN"],
	[FmodDSP.DSP_TYPE_THREE_EQ, "THREE_EQ"],
	[FmodDSP.DSP_TYPE_FFT, "FFT"],
	[FmodDSP.DSP_TYPE_LOUDNESS_METER, "LOUDNESS_METER"],
	[FmodDSP.DSP_TYPE_CONVOLUTIONREVERB, "CONVOLUTIONREVERB"],
	[FmodDSP.DSP_TYPE_CHANNELMIX, "CHANNELMIX"],
	[FmodDSP.DSP_TYPE_TRANSCEIVER, "TRANSCEIVER"],
	[FmodDSP.DSP_TYPE_OBJECTPAN, "OBJECTPAN"],
	[FmodDSP.DSP_TYPE_MULTIBAND_EQ, "MULTIBAND_EQ"],
	[FmodDSP.DSP_TYPE_MULTIBAND_DYNAMICS, "MULTIBAND_DYNAMICS"],
]

var _log_file: FileAccess

func _log(message: String) -> void:
	print(message)
	if _log_file != null:
		_log_file.store_line(message)
		_log_file.flush()

func _init() -> void:
	_log_file = FileAccess.open("res://tests/dsp_parameter_info_result.txt", FileAccess.WRITE)
	_log("DSP_PARAMETER_INFO_TEST_BEGIN")

	var system := FmodServer.get_main_system()
	if system == null:
		push_error("DSP_PARAMETER_INFO_TEST_FATAL: FmodServer.get_main_system() returned null")
		quit(2)
		return

	var create_failures := 0
	var empty_infos := 0
	var total_infos := 0

	for item in DSP_TYPES:
		var dsp_type: int = item[0]
		var dsp_name: String = item[1]
		_log("DSP_BEGIN %s %d" % [dsp_name, dsp_type])

		var dsp := system.create_dsp_by_type(dsp_type)
		if dsp == null:
			create_failures += 1
			_log("DSP_CREATE_FAILED %s %d" % [dsp_name, dsp_type])
			continue

		var parameter_count := dsp.get_num_parameters()
		_log("DSP_PARAMETER_COUNT %s %d" % [dsp_name, parameter_count])

		for index in range(parameter_count):
			var info: Dictionary = dsp.get_parameter_info(index)
			total_infos += 1

			if info.is_empty():
				empty_infos += 1
				_log("DSP_PARAMETER_EMPTY %s %d" % [dsp_name, index])
				continue

			_log("DSP_PARAMETER_OK %s %d %s %s" % [
				dsp_name,
				index,
				str(info.get("type", "")),
				str(info.get("name", "")),
			])

		dsp.release()
		_log("DSP_END %s" % dsp_name)

	_log("DSP_PARAMETER_INFO_TEST_SUMMARY created_failures=%d empty_infos=%d total_infos=%d" % [
		create_failures,
		empty_infos,
		total_infos,
	])
	_log("DSP_PARAMETER_INFO_TEST_END")

	quit(0)
