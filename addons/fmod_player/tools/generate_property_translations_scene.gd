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

var _locale_line_edit: LineEdit
var _add_row_button: Button
var _add_column_button: Button
var _delete_row_button: Button
var _delete_column_button: Button

func _ready() -> void:
	_ensure_toolbar()
	_update_path_labels()
	_build_tree()
	_set_status("Ready. The scene will generate the translation script from CSV.")
	if auto_generate_on_ready:
		call_deferred("_generate")

func _ensure_toolbar() -> void:
	var layout: VBoxContainer = _csv_edit_tree.get_parent()
	var existing: HBoxContainer = layout.get_node_or_null("CSVToolbar") as HBoxContainer
	if existing != null:
		_bind_toolbar(existing)
		return

	var toolbar := HBoxContainer.new()
	toolbar.name = "CSVToolbar"
	toolbar.add_theme_constant_override("separation", 8)
	layout.add_child(toolbar)
	layout.move_child(toolbar, _csv_edit_tree.get_index())

	_add_row_button = Button.new()
	_add_row_button.name = "AddRowButton"
	_add_row_button.text = "Insert Row"
	toolbar.add_child(_add_row_button)

	_locale_line_edit = LineEdit.new()
	_locale_line_edit.name = "LocaleLineEdit"
	_locale_line_edit.placeholder_text = "Locale, e.g. ja or zh_TW"
	_locale_line_edit.text = _make_unique_locale("new_locale")
	_locale_line_edit.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	toolbar.add_child(_locale_line_edit)

	_add_column_button = Button.new()
	_add_column_button.name = "AddColumnButton"
	_add_column_button.text = "Add Column"
	toolbar.add_child(_add_column_button)

	_delete_row_button = Button.new()
	_delete_row_button.name = "DeleteRowButton"
	_delete_row_button.text = "Delete Row"
	toolbar.add_child(_delete_row_button)

	_delete_column_button = Button.new()
	_delete_column_button.name = "DeleteColumnButton"
	_delete_column_button.text = "Delete Last Column"
	toolbar.add_child(_delete_column_button)

	_bind_toolbar(toolbar)

func _bind_toolbar(toolbar: HBoxContainer) -> void:
	_add_row_button = toolbar.get_node_or_null("AddRowButton") as Button
	_locale_line_edit = toolbar.get_node_or_null("LocaleLineEdit") as LineEdit
	_add_column_button = toolbar.get_node_or_null("AddColumnButton") as Button
	_delete_row_button = toolbar.get_node_or_null("DeleteRowButton") as Button
	_delete_column_button = toolbar.get_node_or_null("DeleteColumnButton") as Button

	if _add_row_button == null:
		_add_row_button = toolbar.get_child(0) as Button
	if _locale_line_edit == null:
		_locale_line_edit = toolbar.get_child(1) as LineEdit
	if _add_column_button == null:
		_add_column_button = toolbar.get_child(2) as Button
	if _delete_row_button == null and toolbar.get_child_count() > 3:
		_delete_row_button = toolbar.get_child(3) as Button
	if _delete_column_button == null and toolbar.get_child_count() > 4:
		_delete_column_button = toolbar.get_child(4) as Button

	if _add_row_button and not _add_row_button.pressed.is_connected(_on_add_row_button_pressed):
		_add_row_button.pressed.connect(_on_add_row_button_pressed)
	if _add_column_button and not _add_column_button.pressed.is_connected(_on_add_column_button_pressed):
		_add_column_button.pressed.connect(_on_add_column_button_pressed)
	if _delete_row_button and not _delete_row_button.pressed.is_connected(_on_delete_row_button_pressed):
		_delete_row_button.pressed.connect(_on_delete_row_button_pressed)
	if _delete_column_button and not _delete_column_button.pressed.is_connected(_on_delete_column_button_pressed):
		_delete_column_button.pressed.connect(_on_delete_column_button_pressed)

func _build_tree() -> void:
	var data: Dictionary = PROPERTY_TRANSLATION_GENERATOR._read_translation_csv(csv_path)
	var locales: Array = data.get("locales", [])
	var messages: Array[Dictionary] = data.get("messages", [])
	_csv_edit_tree.clear()
	_csv_edit_tree.columns = locales.size() + 1

	_csv_edit_tree.set_column_title(0, "Source")
	_csv_edit_tree.set_column_expand(0, true)
	for index: int in range(locales.size()):
		var locale: String = String(locales[index])
		_csv_edit_tree.set_column_title(index + 1, locale)
		_csv_edit_tree.set_column_expand(index + 1, true)

	var root: TreeItem = _csv_edit_tree.create_item()
	for message: Dictionary in messages:
		var item: TreeItem = _csv_edit_tree.create_item(root)
		var source: String = message.get("source", String())
		var translations: Dictionary = message.get("translations", Dictionary())
		_setup_row_item(item, source, translations)

func _setup_row_item(item: TreeItem, source: String, translations: Dictionary = Dictionary()) -> void:
	item.set_text(0, source)
	item.set_editable(0, true)

	for column: int in range(1, _csv_edit_tree.columns):
		var locale: String = _csv_edit_tree.get_column_title(column)
		item.set_text(column, translations.get(locale, String()))
		item.set_editable(column, true)

func _save_tree_to_csv() -> void:
	var root: TreeItem = _csv_edit_tree.get_root()
	if root == null:
		return

	var columns: int = _csv_edit_tree.columns
	var locales: PackedStringArray = PackedStringArray()

	for i: int in range(1, columns):
		var locale: String = _csv_edit_tree.get_column_title(i).strip_edges()
		if not locale.is_empty():
			locales.append(locale)

	var messages: Array[Dictionary] = []
	var item: TreeItem = root.get_first_child()

	while item != null:
		var source: String = item.get_text(0).strip_edges()
		var translations: Dictionary = {}
		if source.is_empty():
			item = item.get_next()
			continue

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

	if PROPERTY_TRANSLATION_GENERATOR._write_translation_csv(csv_path, data):
		_set_status("Saved CSV.")
	else:
		_set_status("Failed to save CSV. Check the Output panel.")

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

func _on_add_row_button_pressed() -> void:
	var root: TreeItem = _csv_edit_tree.get_root()
	if root == null:
		root = _csv_edit_tree.create_item()

	var insert_index := -1
	var selected: TreeItem = _csv_edit_tree.get_selected()
	if selected != null and selected.get_parent() == root:
		insert_index = _get_item_index(selected) + 1

	var item: TreeItem = _csv_edit_tree.create_item(root, insert_index)
	_setup_row_item(item, _make_unique_source("New Source"))
	item.select(0)
	_set_status("Inserted a new row.")

func _on_add_column_button_pressed() -> void:
	var locale := _locale_line_edit.text.strip_edges() if _locale_line_edit else String()
	if locale.is_empty():
		locale = "new_locale"

	locale = _make_unique_locale(locale)
	var column := _csv_edit_tree.columns
	_csv_edit_tree.columns = column + 1
	_csv_edit_tree.set_column_title(column, locale)
	_csv_edit_tree.set_column_expand(column, true)

	var root: TreeItem = _csv_edit_tree.get_root()
	var item: TreeItem = root.get_first_child() if root else null
	while item != null:
		item.set_text(column, String())
		item.set_editable(column, true)
		item = item.get_next()

	if _locale_line_edit:
		_locale_line_edit.text = _make_unique_locale(locale)
	_set_status("Added locale column: %s" % locale)

func _on_delete_row_button_pressed() -> void:
	var selected: TreeItem = _csv_edit_tree.get_selected()
	if selected == null or selected == _csv_edit_tree.get_root():
		_set_status("Select a row before deleting.")
		return

	selected.free()
	_set_status("Deleted selected row.")

func _on_delete_column_button_pressed() -> void:
	if _csv_edit_tree.columns <= 2:
		_set_status("At least one locale column must remain.")
		return

	var removed_locale := _csv_edit_tree.get_column_title(_csv_edit_tree.columns - 1)
	_csv_edit_tree.columns -= 1
	_set_status("Deleted last locale column: %s" % removed_locale)

func _get_item_index(target: TreeItem) -> int:
	var index := 0
	var item: TreeItem = target.get_parent().get_first_child()
	while item != null:
		if item == target:
			return index
		index += 1
		item = item.get_next()
	return -1

func _make_unique_source(base: String) -> String:
	var existing := {}
	var root: TreeItem = _csv_edit_tree.get_root()
	var item: TreeItem = root.get_first_child() if root else null
	while item != null:
		existing[item.get_text(0)] = true
		item = item.get_next()

	var candidate := base
	var suffix := 2
	while existing.has(candidate):
		candidate = "%s %d" % [base, suffix]
		suffix += 1
	return candidate

func _make_unique_locale(base: String) -> String:
	var normalized := base.strip_edges().replace(" ", "_")
	if normalized.is_empty():
		normalized = "new_locale"

	var existing := {}
	for column: int in range(1, _csv_edit_tree.columns if _csv_edit_tree else 1):
		existing[_csv_edit_tree.get_column_title(column)] = true

	var candidate := normalized
	var suffix := 2
	while existing.has(candidate):
		candidate = "%s_%d" % [normalized, suffix]
		suffix += 1
	return candidate
