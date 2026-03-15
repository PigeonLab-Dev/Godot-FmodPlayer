@tool
class_name FmodListener3D
extends Node3D

# FMOD 3D 听者更新器
# 将此脚本附加到 Camera3D 或玩家角色上
# 会自动更新 FMOD 的听者位置

@export var listener_index: int = 0
@export var velocity_tracking: bool = true

var last_position: Vector3
var current_velocity: Vector3 = Vector3.ZERO

func _ready():
	last_position = global_position

func _process(delta):
	if velocity_tracking:
		current_velocity = (global_position - last_position) / delta
		last_position = global_position
	
	_update_fmod_listener()

func _update_fmod_listener():
	var system = FmodServer.get_main_system()
	if not system:
		return
	
	# 获取朝向
	var forward = -global_transform.basis.z.normalized()
	var up = global_transform.basis.y.normalized()
	
	# 更新 FMOD 听者位置
	system.set_3d_listener_attributes(
		listener_index,
		global_position,
		current_velocity,
		forward,
		up
	)

func _exit_tree():
	# 重置听者位置（可选）
	var system = FmodServer.get_main_system()
	if system:
		system.set_3d_listener_attributes(
			listener_index,
			Vector3.ZERO,
			Vector3.ZERO,
			Vector3.FORWARD,
			Vector3.UP
		)
