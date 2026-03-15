#ifndef FMOD_GEOMETRY_INSTANCE_3D_H
#define FMOD_GEOMETRY_INSTANCE_3D_H

#include "core/fmod_utils.hpp"
#include "geometry/geometry.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {
	// 前向声明
	class MeshInstance3D;
	class CollisionShape3D;

	/**
	 * FMOD Geometry Instance 3D
	 * 
	 * 专门附加于 MeshInstance3D 或 CollisionShape3D 的节点，
	 * 自动从父节点创建 FMOD 音频遮挡几何体。
	 * 
	 * 使用方法：
	 * 1. 将本节点作为 MeshInstance3D 或 CollisionShape3D 的子节点
	 * 2. 进入场景时自动创建 FmodGeometry
	 * 3. 可通过属性面板调整遮挡参数
	 */
	class FmodGeometryInstance3D : public Node3D {
		GDCLASS(FmodGeometryInstance3D, Node3D)

	private:
		// 核心对象
		Ref<FmodGeometry> geometry;
		
		// 导出属性
		float direct_occlusion = 0.5f;
		float reverb_occlusion = 0.5f;
		bool double_sided = true;
		bool active = true;
		bool auto_rebuild = true;
		bool show_debug_gizmo = true;
		
		// 源节点缓存（避免频繁查找）
		NodePath source_path;
		MeshInstance3D* cached_mesh_instance = nullptr;
		CollisionShape3D* cached_collision_shape = nullptr;
		
		// 网格变化检测
		uint32_t last_mesh_version = 0;

	protected:
		static void _bind_methods();
		void _notification(int p_what);

	public:
		FmodGeometryInstance3D();
		~FmodGeometryInstance3D();

		// 生命周期
		void _enter_tree() override;
		void _ready() override;
		void _exit_tree() override;
		void _process(double delta) override;

		// 核心功能
		void rebuild_geometry();
		void clear_geometry();
		bool has_valid_geometry() const;
		
		// 源节点检测
		void detect_source_node();
		bool is_valid_source_node(Node* node) const;
		
		// 几何体构建
		bool build_from_mesh_instance(MeshInstance3D* mesh_instance);
		bool build_from_collision_shape(CollisionShape3D* collision_shape);
		bool build_from_mesh(Ref<Mesh> mesh, const Transform3D& transform);
		
		// 工具函数
		static PackedVector3Array get_mesh_triangles(Ref<Mesh> mesh);
		static Vector3 godot_to_fmod_vector(const Vector3& v);
		static Vector3 fmod_to_godot_vector(const Vector3& v);
		
		void set_direct_occlusion(float p_value);
		float get_direct_occlusion() const;
		
		void set_reverb_occlusion(float p_value);
		float get_reverb_occlusion() const;
		
		void set_double_sided(bool p_value);
		bool get_double_sided() const;
		
		void set_active(bool p_value);
		bool get_active() const;
		
		void set_auto_rebuild(bool p_value);
		bool get_auto_rebuild() const;
		
		void set_show_debug_gizmo(bool p_value);
		bool get_show_debug_gizmo() const;
		
		// 获取内部对象
		Ref<FmodGeometry> get_geometry() const;
	};
}

#endif // FMOD_GEOMETRY_INSTANCE_3D_H
