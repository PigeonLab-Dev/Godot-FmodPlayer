@tool
extends SceneTree

const PropertyTranslationGenerator = preload("res://addons/fmod_player/tools/property_translation_generator.gd")

func _init() -> void:
	var options: Dictionary = _parse_user_args()
	var csv_path: String = String(options.get("csv", PropertyTranslationGenerator.DEFAULT_CSV_PATH))
	var output_path: String = String(options.get("out", PropertyTranslationGenerator.DEFAULT_OUTPUT_PATH))

	if not PropertyTranslationGenerator.generate(csv_path, output_path):
		quit(1)
		return

	quit()

func _parse_user_args() -> Dictionary:
	var result: Dictionary = _parse_arg_list(OS.get_cmdline_user_args())
	if not result.is_empty():
		return result

	return _parse_arg_list(OS.get_cmdline_args())

func _parse_arg_list(args: PackedStringArray) -> Dictionary:
	var result: Dictionary
	var index: int = 0
	while index < args.size():
		var arg: String = args[index]
		if arg == "--csv" and index + 1 < args.size():
			result["csv"] = args[index + 1]
			index += 2
			continue
		if arg == "--out" and index + 1 < args.size():
			result["out"] = args[index + 1]
			index += 2
			continue
		index += 1
	return result
