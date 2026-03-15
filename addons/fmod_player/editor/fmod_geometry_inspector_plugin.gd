@tool
extends EditorInspectorPlugin

# FMOD Geometry 检查器插件
# 在 MeshInstance3D 的属性面板中添加 FMOD 几何体生成按钮

func _can_handle(object) -> bool:
	# 处理 MeshInstance3D 和 FmodGeometry3D
	return object is MeshInstance3D or object is FmodGeometry3D

func _parse_begin(object):
	if object is MeshInstance3D:
		_add_mesh_instance_controls(object)
	elif object is FmodGeometry3D:
		_add_geometry_controls(object)

func _add_mesh_instance_controls(mesh_instance: MeshInstance3D):
	var container = VBoxContainer.new()
	
	# 标题
	var title = Label.new()
	title.text = "FMOD 音频几何体"
	title.add_theme_font_size_override("font_size", 14)
	title.add_theme_color_override("font_color", Color(0.2, 0.8, 1.0))
	container.add_child(title)
	
	# 检查是否已有几何体
	var has_geometry = _has_fmod_geometry(mesh_instance)
	
	if has_geometry:
		var info = Label.new()
		info.text = "✓ 已生成音频几何体"
		info.add_theme_color_override("font_color", Color(0.2, 0.9, 0.2))
		container.add_child(info)
		
		var edit_btn = Button.new()
		edit_btn.text = "编辑音频属性"
		edit_btn.pressed.connect(_edit_geometry.bind(mesh_instance))
		container.add_child(edit_btn)
		
		var remove_btn = Button.new()
		remove_btn.text = "移除音频几何体"
		remove_btn.pressed.connect(_remove_geometry.bind(mesh_instance))
		remove_btn.add_theme_color_override("font_color", Color(0.9, 0.2, 0.2))
		container.add_child(remove_btn)
	else:
		var desc = Label.new()
		desc.text = "为此网格生成声音遮挡几何体"
		desc.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
		container.add_child(desc)
		
		var generate_btn = Button.new()
		generate_btn.text = "生成音频几何体"
		generate_btn.icon = preload("res://addons/fmod_player/icons/fmod_geometry_icon.svg")
		generate_btn.pressed.connect(_generate_geometry.bind(mesh_instance))
		container.add_child(generate_btn)
		
		# 快速设置
		var quick_settings = HBoxContainer.new()
		
		var solid_btn = Button.new()
		solid_btn.text = "实体墙"
		solid_btn.tooltip_text = "高遮挡 (direct: 0.95)"
		solid_btn.pressed.connect(_generate_with_preset.bind(mesh_instance, 0.95, 0.9))
		quick_settings.add_child(solid_btn)
		
		var thin_btn = Button.new()
		thin_btn.text = "薄墙"
		thin_btn.tooltip_text = "中等遮挡 (direct: 0.6)"
		thin_btn.pressed.connect(_generate_with_preset.bind(mesh_instance, 0.6, 0.5))
		quick_settings.add_child(thin_btn)
		
		var fence_btn = Button.new()
		fence_btn.text = "栅栏"
		fence_btn.tooltip_text = "低遮挡 (direct: 0.3)"
		fence_btn.pressed.connect(_generate_with_preset.bind(mesh_instance, 0.3, 0.2))
		quick_settings.add_child(fence_btn)
		
		container.add_child(quick_settings)
	
	add_custom_control(container)

func _add_geometry_controls(geometry: FmodGeometry3D):
	var container = VBoxContainer.new()
	
	var title = Label.new()
	title.text = "FMOD 几何体属性"
	title.add_theme_font_size_override("font_size", 14)
	container.add_child(title)
	
	# 重新生成按钮
	var regen_btn = Button.new()
	regen_btn.text = "从源网格重新生成"
	regen_btn.pressed.connect(func():
		geometry.rebuild_from_mesh()
	)
	container.add_child(regen_btn)
	
	add_custom_control(container)

func _has_fmod_geometry(mesh_instance: MeshInstance3D) -> bool:
	for child in mesh_instance.get_children():
		if child is FmodGeometry3D:
			return true
	return false

func _generate_geometry(mesh_instance: MeshInstance3D):
	var plugin = EditorInterface.get_editor_plugin()
	if plugin.has_method("_create_geometry_for_mesh"):
		plugin._create_geometry_for_mesh(mesh_instance)
	EditorInterface.get_inspector().refresh()

func _generate_with_preset(mesh_instance: MeshInstance3D, direct: float, reverb: float):
	var geom = FmodGeometry3D.new()
	geom.name = mesh_instance.name + "_AudioGeom"
	geom.mesh_source = NodePath(mesh_instance.name)
	geom.direct_occlusion = direct
	geom.reverb_occlusion = reverb
	
	mesh_instance.add_child(geom)
	geom.owner = EditorInterface.get_edited_scene_root()
	
	EditorInterface.get_inspector().refresh()

func _edit_geometry(mesh_instance: MeshInstance3D):
	# 选中间第一个几何体子节点
	for child in mesh_instance.get_children():
		if child is FmodGeometry3D:
			EditorInterface.get_selection().clear()
			EditorInterface.get_selection().add_node(child)
			return

func _remove_geometry(mesh_instance: MeshInstance3D):
	for child in mesh_instance.get_children():
		if child is FmodGeometry3D:
			child.queue_free()
			break
	EditorInterface.get_inspector().refresh()
