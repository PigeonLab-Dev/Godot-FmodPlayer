extends Node
class_name FmodAutoListener

# 自动 FMOD 听者更新器
# 将此节点添加到场景任意位置，它会自动：
# 1. 查找当前视口的激活 Camera3D
# 2. 每帧更新 FMOD 听者位置

@export var listener_index: int = 0
@export var enabled: bool = true
@export var track_velocity: bool = false

var _last_position: Vector3
var _current_velocity: Vector3

func _ready():
	# 设置为处理模式
	set_process(true)

func _process(delta):
	if not enabled:
		return
	
	# 获取当前视口的 Camera3D
	var viewport = get_viewport()
	if not viewport:
		return
	
	var camera = viewport.get_camera_3d()
	if not camera:
		return
	
	# 计算速度
	var pos = camera.global_position
	if track_velocity:
		_current_velocity = (pos - _last_position) / delta
	_last_position = pos
	
	# 获取朝向
	var forward = -camera.global_transform.basis.z.normalized()
	var up = camera.global_transform.basis.y.normalized()
	
	# 更新 FMOD 听者
	var system = FmodServer.get_main_system()
	if system:
		system.set_3d_listener_attributes(
			listener_index,
			pos,
			_current_velocity if track_velocity else Vector3.ZERO,
			forward,
			up
		)

func get_current_camera() -> Camera3D:
	var viewport = get_viewport()
	if viewport:
		return viewport.get_camera_3d()
	return null
