extends Control

const PROPERTY_TRANSLATION_GENERATOR = preload("res://addons/fmod_player/tools/property_translation_generator.gd")

@export_file("*.csv") var csv_path: String = PROPERTY_TRANSLATION_GENERATOR.DEFAULT_CSV_PATH
@export_file("*.gd") var output_path: String = PROPERTY_TRANSLATION_GENERATOR.DEFAULT_OUTPUT_PATH
@export var auto_generate_on_ready: bool = true

@onready var _csv_label: Label = %CSVLabel
@onready var _csv_edit_tree: Tree = %CSVEditTree
@onready var _output_label: Label = %OutputLabel
@onready var _status_label: Label = %StatusLabel
@onready var _generate_button: Button = %GenerateButton

func _ready() -> void:
	_update_path_labels()
	_build_tree()
	_set_status("Ready. The scene will generate the translation script from CSV.")
	if auto_generate_on_ready:
		call_deferred("_generate")

func _build_tree() -> void:
	var data: Dictionary = PROPERTY_TRANSLATION_GENERATOR._read_translation_csv(PROPERTY_TRANSLATION_GENERATOR.DEFAULT_CSV_PATH)
	var locales: PackedStringArray = data.get("locales", PackedStringArray())
	var messages: Array[Dictionary] = data.get("messages", [])
	_csv_edit_tree.columns = locales.size() + 1
	
	_csv_edit_tree.set_column_title(0, "Source")
	for index: int in locales.size():
		var local: String = locales[index]
		_csv_edit_tree.set_column_title(index + 1, local)
		
	var root: TreeItem = _csv_edit_tree.create_item()
	for message: Dictionary in messages:
		var item: TreeItem = _csv_edit_tree.create_item(root)
		var source: String = message.get("source", String())
		item.set_text(0, source)
		item.set_editable(0, true)
			
		var translations: Dictionary = message.get("translations", Dictionary())
		var count: int = 1
		for key: String in translations.keys():
			item.set_text(count, translations.get(key, String()))
			item.set_editable(count, true)
			count += 1

func _save_tree_to_csv() -> void:
	var root: TreeItem = _csv_edit_tree.get_root()
	if root == null:
		return
	
	var columns: int = _csv_edit_tree.columns
	var locales: PackedStringArray = PackedStringArray()
	
	for i: int in range(1, columns):
		locales.append(_csv_edit_tree.get_column_title(i))
	
	var messages: Array[Dictionary] = []
	var item: TreeItem = root.get_first_child()
	
	while item != null:
		var source: String = item.get_text(0)
		var translations: Dictionary = {}
		
		for i: int in range(1, columns):
			var locale: String = locales[i - 1]
			var text: String = item.get_text(i)
			translations[locale] = text
		
		messages.append({
			"source": source,
			"translations": translations
		})
		
		item = item.get_next()
	
	var data: Dictionary = {
		"locales": locales,
		"messages": messages
	}
	
	PROPERTY_TRANSLATION_GENERATOR._write_translation_csv(csv_path, data)

func _generate() -> void:
	if _generate_button:
		_generate_button.disabled = true

	_update_path_labels()
	_set_status("Generating...")

	var ok: bool = PROPERTY_TRANSLATION_GENERATOR.generate(csv_path, output_path)
	if ok:
		_set_status("Done. Generated translation script successfully.")
	else:
		_set_status("Failed. Check the Output panel for CSV or path errors.")

	if _generate_button:
		_generate_button.disabled = false

func _update_path_labels() -> void:
	if _csv_label:
		_csv_label.text = "CSV: %s" % csv_path
	if _output_label:
		_output_label.text = "Output: %s" % output_path

func _set_status(text: String) -> void:
	if _status_label:
		_status_label.text = "Status: %s" % text
	print("FMOD translation generator: %s" % text)

func _on_generate_button_pressed() -> void:
	_save_tree_to_csv()
	_generate()
