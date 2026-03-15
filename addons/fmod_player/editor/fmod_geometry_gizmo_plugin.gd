@tool
extends EditorNode3DGizmoPlugin

# FMOD Geometry Gizmo 插件
# 在 3D 视图中可视化音频几何体

const GIZMO_COLOR = Color(0.2, 0.8, 1.0, 0.6)
const GIZMO_COLOR_ACTIVE = Color(0.2, 1.0, 0.4, 0.8)

func _init():
	create_material("fmod_geometry", GIZMO_COLOR)
	create_material("fmod_geometry_active", GIZMO_COLOR_ACTIVE)
	create_handle_material("handles")

func _get_gizmo_name() -> String:
	return "FmodGeometry"

func _has_gizmo(node) -> bool:
	return node is FmodGeometry3D

func _is_selectable_when_hidden() -> bool:
	return true

func _redraw(gizmo):
	gizmo.clear()
	
	var node = gizmo.get_node_3d()
	if not node is FmodGeometry3D:
		return
	
	# 获取源网格的 AABB
	var aabb = _get_mesh_aabb(node)
	if aabb.size.length() == 0:
		return
	
	# 绘制 AABB 框线
	var lines = _create_aabb_lines(aabb)
	
	var material = get_material("fmod_geometry_active" if node.active else "fmod_geometry", gizmo)
	gizmo.add_lines(lines, material)
	
	# 如果启用了调试显示，绘制更多细节
	if node.show_debug_outline:
		_draw_detailed_outline(gizmo, node, material)

func _get_mesh_aabb(node: FmodGeometry3D) -> AABB:
	# 尝试从源 MeshInstance3D 获取 AABB
	if node.mesh_source:
		var parent = node.get_node_or_null(node.mesh_source)
		if parent is MeshInstance3D and parent.mesh:
			return parent.mesh.get_aabb()
	
	# 回退：计算子节点几何体的 AABB
	return AABB(Vector3.ZERO, Vector3.ONE)

func _create_aabb_lines(aabb: AABB) -> PackedVector3Array:
	var lines = PackedVector3Array()
	var min_v = aabb.position
	var max_v = aabb.position + aabb.size
	
	# 底面
	lines.append(Vector3(min_v.x, min_v.y, min_v.z))
	lines.append(Vector3(max_v.x, min_v.y, min_v.z))
	
	lines.append(Vector3(max_v.x, min_v.y, min_v.z))
	lines.append(Vector3(max_v.x, min_v.y, max_v.z))
	
	lines.append(Vector3(max_v.x, min_v.y, max_v.z))
	lines.append(Vector3(min_v.x, min_v.y, max_v.z))
	
	lines.append(Vector3(min_v.x, min_v.y, max_v.z))
	lines.append(Vector3(min_v.x, min_v.y, min_v.z))
	
	# 顶面
	lines.append(Vector3(min_v.x, max_v.y, min_v.z))
	lines.append(Vector3(max_v.x, max_v.y, min_v.z))
	
	lines.append(Vector3(max_v.x, max_v.y, min_v.z))
	lines.append(Vector3(max_v.x, max_v.y, max_v.z))
	
	lines.append(Vector3(max_v.x, max_v.y, max_v.z))
	lines.append(Vector3(min_v.x, max_v.y, max_v.z))
	
	lines.append(Vector3(min_v.x, max_v.y, max_v.z))
	lines.append(Vector3(min_v.x, max_v.y, min_v.z))
	
	# 垂直边
	lines.append(Vector3(min_v.x, min_v.y, min_v.z))
	lines.append(Vector3(min_v.x, max_v.y, min_v.z))
	
	lines.append(Vector3(max_v.x, min_v.y, min_v.z))
	lines.append(Vector3(max_v.x, max_v.y, min_v.z))
	
	lines.append(Vector3(max_v.x, min_v.y, max_v.z))
	lines.append(Vector3(max_v.x, max_v.y, max_v.z))
	
	lines.append(Vector3(min_v.x, min_v.y, max_v.z))
	lines.append(Vector3(min_v.x, max_v.y, max_v.z))
	
	return lines

func _draw_detailed_outline(gizmo, node: FmodGeometry3D, material):
	# 如果可能，绘制实际的网格轮廓
	if node.geometry and node.geometry.is_valid():
		# 这里可以添加更详细的轮廓绘制
		# 需要从 FMOD::Geometry 获取多边形数据
		pass
