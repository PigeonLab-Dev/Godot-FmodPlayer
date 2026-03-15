extends Camera3D
# 将此脚本附加到主 Camera3D 上
# 会自动每帧更新 FMOD 听者位置

func _process(_delta):
	var system = FmodServer.get_main_system()
	if not system:
		return
	
	# 获取当前 Camera3D 的变换
	var pos = global_position
	var forward = -global_transform.basis.z.normalized()
	var up = global_transform.basis.y.normalized()
	
	# 计算速度（可选）
	var velocity = Vector3.ZERO
	
	# 更新 FMOD 听者 0 号位置
	system.set_3d_listener_attributes(0, pos, velocity, forward, up)
