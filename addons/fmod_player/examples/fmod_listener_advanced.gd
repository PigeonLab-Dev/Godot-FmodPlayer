extends Node3D
class_name FmodListenerAdvanced

# 高级 FMOD 听者管理器
# 支持：
# - 自动检测 Camera3D 或 AudioListener3D
# - 多监听者配置
# - 平滑速度跟踪

@export var listener_index: int = 0
@export var auto_detect_camera: bool = true
@export var velocity_smoothing: float = 0.1
@export var track_velocity: bool = false

var _last_position: Vector3
var _smoothed_velocity: Vector3
var _target_node: Node3D

func _ready():
	# 尝试找到监听目标
	if auto_detect_camera:
		# 首先检查自己是否已经是 Camera3D
		if self is Camera3D:
			_target_node = self
		else:
			# 查找同级或父级的 Camera3D
			_target_node = _find_camera(get_parent())
			
		if not _target_node:
			push_warning("FmodListenerAdvanced: No Camera3D found")
	
	if _target_node:
		_last_position = _target_node.global_position

func _find_camera(node: Node) -> Camera3D:
	if node is Camera3D:
		return node
	
	for child in node.get_children():
		if child is Camera3D:
			return child
		var found = _find_camera(child)
		if found:
			return found
	
	return null

func _process(delta):
	if not _target_node:
		return
	
	var system = FmodServer.get_main_system()
	if not system:
		return
	
	var pos = _target_node.global_position
	var forward = -_target_node.global_transform.basis.z.normalized()
	var up = _target_node.global_transform.basis.y.normalized()
	
	var velocity = Vector3.ZERO
	if track_velocity:
		var instant_velocity = (pos - _last_position) / delta
		_smoothed_velocity = _smoothed_velocity.lerp(instant_velocity, velocity_smoothing)
		velocity = _smoothed_velocity
	
	_last_position = pos
	
	# 更新 FMOD 听者
	system.set_3d_listener_attributes(listener_index, pos, velocity, forward, up)

func set_target_node(node: Node3D):
	_target_node = node
	_last_position = node.global_position if node else Vector3.ZERO
