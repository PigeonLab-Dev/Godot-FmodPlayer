@tool
extends EditorPlugin

# This is the main plugin that actually uses GDExtension classes.
# It will only be loaded after confirming fmod.dll exists.

var importer
var inspector
var fmod_audio_stream_player_3d_gizom_plugin
var _editor_interface: EditorInterface

func set_editor_interface(iface: EditorInterface):
	_editor_interface = iface

func get_editor_interface() -> EditorInterface:
	return _editor_interface

func _enter_tree():
	# Use load() instead of preload to ensure GDExtension is loaded
	var importer_class = load("res://addons/fmod_player/bin/fmod_player.gdextension")
	
	# Dynamically instantiate classes
	if ClassDB.class_exists("AudioImporterFmod"):
		importer = ClassDB.instantiate("AudioImporterFmod")
		add_import_plugin(importer)
	
	if ClassDB.class_exists("FmodAudioPreviewInspector"):
		inspector = ClassDB.instantiate("FmodAudioPreviewInspector")
		add_inspector_plugin(inspector)
	
	if ClassDB.class_exists("FmodAudioStreamPlayer3DGizmoPlugin"):
		fmod_audio_stream_player_3d_gizom_plugin = ClassDB.instantiate("FmodAudioStreamPlayer3DGizmoPlugin")
		add_node_3d_gizmo_plugin(fmod_audio_stream_player_3d_gizom_plugin)

func _exit_tree():
	if importer:
		remove_import_plugin(importer)
		importer = null
	if inspector:
		remove_inspector_plugin(inspector)
		inspector = null
	if fmod_audio_stream_player_3d_gizom_plugin:
		remove_node_3d_gizmo_plugin(fmod_audio_stream_player_3d_gizom_plugin)
		fmod_audio_stream_player_3d_gizom_plugin = null
