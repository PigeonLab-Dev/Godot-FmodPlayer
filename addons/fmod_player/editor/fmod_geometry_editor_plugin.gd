@tool
extends EditorPlugin

# FMOD Geometry 编辑器插件
# 为 MeshInstance3D 添加一键生成音频几何体的功能

var toolbar_button: MenuButton
var gizmo_plugin

func _enter_tree():
	# 添加自定义节点类型
	add_custom_type(
		"FmodGeometry3D",
		"Node3D",
		preload("res://addons/fmod_player/nodes/fmod_geometry_3d.gd"),
		preload("res://addons/fmod_player/icons/fmod_geometry_icon.svg")
	)
	
	# 添加编辑器检查器插件
	add_inspector_plugin(preload("res://addons/fmod_player/editor/fmod_geometry_inspector_plugin.gd").new())
	
	# 添加工具栏按钮
	_add_toolbar_button()
	
	# 注册 Gizmo
	gizmo_plugin = preload("res://addons/fmod_player/editor/fmod_geometry_gizmo_plugin.gd").new()
	add_node_3d_gizmo_plugin(gizmo_plugin)

func _exit_tree():
	remove_custom_type("FmodGeometry3D")
	_remove_toolbar_button()
	remove_node_3d_gizmo_plugin(gizmo_plugin)

func _add_toolbar_button():
	toolbar_button = MenuButton.new()
	toolbar_button.text = "FMOD Audio"
	toolbar_button.tooltip_text = "FMOD 音频几何体工具"
	
	var popup = toolbar_button.get_popup()
	popup.add_item("从选中生成音频几何体", 0)
	popup.set_item_tooltip(0, "为选中的 MeshInstance3D 生成声音遮挡几何体")
	
	popup.add_item("批量生成 (所有 MeshInstance)", 1)
	popup.set_item_tooltip(1, "为场景中所有 MeshInstance3D 生成音频几何体")
	
	popup.add_separator()
	
	popup.add_item("查看音频轮廓 (调试)", 2)
	popup.set_item_tooltip(2, "在 3D 视图中显示音频几何体轮廓")
	popup.add_check_item("显示调试轮廓", 3)
	popup.set_item_tooltip(3, "持续显示所有音频几何体的轮廓")
	
	popup.add_separator()
	
	popup.add_item("清除选中几何体", 4)
	popup.set_item_tooltip(4, "删除选中的 FmodGeometry3D 节点")
	popup.add_item("清除所有几何体", 5)
	popup.set_item_tooltip(5, "删除场景中所有 FmodGeometry3D 节点")
	
	popup.id_pressed.connect(_on_menu_pressed)
	
	# 添加到 3D 编辑器工具栏
	add_control_to_container(CONTAINER_SPATIAL_EDITOR_MENU, toolbar_button)

func _remove_toolbar_button():
	if toolbar_button:
		remove_control_from_container(CONTAINER_SPATIAL_EDITOR_MENU, toolbar_button)
		toolbar_button.queue_free()
		toolbar_button = null

func _on_menu_pressed(id: int):
	match id:
		0: _generate_from_selected()
		1: _generate_all_mesh_instances()
		2: _toggle_debug_view()
		3: _toggle_persistent_debug()
		4: _clear_selected_geometry()
		5: _clear_all_geometry()

func _generate_from_selected():
	var selected = EditorInterface.get_selection().get_selected_nodes()
	var count = 0
	
	for node in selected:
		if node is MeshInstance3D:
			_create_geometry_for_mesh(node)
			count += 1
	
	if count == 0:
		push_warning("请先选择至少一个 MeshInstance3D 节点")
	else:
		print_rich("[color=green]✓[/color] 已为 %d 个网格生成 FMOD 音频几何体" % count)

func _generate_all_mesh_instances():
	var root = EditorInterface.get_edited_scene_root()
	if not root:
		push_warning("请先打开一个场景")
		return
	
	var count = 0
	var mesh_instances = _find_all_mesh_instances(root)
	
	for mesh_instance in mesh_instances:
		# 检查是否已有几何体
		var has_geom = false
		for child in mesh_instance.get_children():
			if child is FmodGeometry3D:
				has_geom = true
				break
		
		if not has_geom:
			_create_geometry_for_mesh(mesh_instance)
			count += 1
	
	print_rich("[color=green]✓[/color] 已为 %d/%d 个网格生成 FMOD 音频几何体" % [count, mesh_instances.size()])

func _find_all_mesh_instances(node: Node, result: Array = []) -> Array:
	if node is MeshInstance3D and node.mesh:
		result.append(node)
	
	for child in node.get_children():
		_find_all_mesh_instances(child, result)
	
	return result

func _create_geometry_for_mesh(mesh_instance: MeshInstance3D):
	var geom = FmodGeometry3D.new()
	geom.name = mesh_instance.name + "_AudioGeom"
	geom.mesh_source = NodePath(mesh_instance.name)
	
	# 根据材质自动判断遮挡属性
	var material = mesh_instance.get_active_material(0)
	if material:
		# 根据材质属性设置合理的默认值
		geom.direct_occlusion = _estimate_occlusion_from_material(material)
		geom.reverb_occlusion = geom.direct_occlusion * 0.8
	else:
		geom.direct_occlusion = 0.5
		geom.reverb_occlusion = 0.5
	
	mesh_instance.add_child(geom)
	geom.owner = EditorInterface.get_edited_scene_root()

func _estimate_occlusion_from_material(material: Material) -> float:
	# 根据材质属性估算遮挡值
	# 这只是一个简单的启发式规则
	
	if material is StandardMaterial3D:
		var std_mat = material as StandardMaterial3D
		
		# 如果是不透明材质，遮挡较高
		if std_mat.transparency == BaseMaterial3D.TRANSPARENCY_DISABLED:
			# 根据金属度和粗糙度调整
			if std_mat.metallic > 0.5:
				return 0.95  # 金属几乎完全阻挡
			elif std_mat.roughness > 0.7:
				return 0.8   # 粗糙表面
			else:
				return 0.7   # 普通表面
		else:
			return 0.3  # 透明材质阻挡较少
	
	return 0.5  # 默认中等阻挡

func _toggle_debug_view():
	# 临时显示选中几何体的轮廓
	var selected = EditorInterface.get_selection().get_selected_nodes()
	for node in selected:
		if node is FmodGeometry3D:
			node.show_debug_outline = true

func _toggle_persistent_debug():
	# 切换全局调试显示
	var root = EditorInterface.get_edited_scene_root()
	if root:
		var show = not root.get_meta("fmod_geometry_debug", false)
		root.set_meta("fmod_geometry_debug", show)
		
		# 更新所有几何体的调试显示
		var geoms = _find_all_fmod_geometry(root)
		for geom in geoms:
			geom.show_debug_outline = show
		
		print_rich("[color=%s]✓[/color] 调试轮廓: %s" % [
			"green" if show else "gray",
			"开启" if show else "关闭"
		])

func _find_all_fmod_geometry(node: Node, result: Array = []) -> Array:
	if node is FmodGeometry3D:
		result.append(node)
	
	for child in node.get_children():
		_find_all_fmod_geometry(child, result)
	
	return result

func _clear_selected_geometry():
	var selected = EditorInterface.get_selection().get_selected_nodes()
	var count = 0
	
	for node in selected:
		if node is FmodGeometry3D:
			node.queue_free()
			count += 1
		elif node is MeshInstance3D:
			# 清除子节点中的几何体
			for child in node.get_children():
				if child is FmodGeometry3D:
					child.queue_free()
					count += 1
	
	print_rich("[color=yellow]✓[/color] 已清除 %d 个音频几何体" % count)

func _clear_all_geometry():
	var root = EditorInterface.get_edited_scene_root()
	if not root:
		return
	
	var geoms = _find_all_fmod_geometry(root)
	for geom in geoms:
		geom.queue_free()
	
	print_rich("[color=yellow]✓[/color] 已清除所有 %d 个音频几何体" % geoms.size())
