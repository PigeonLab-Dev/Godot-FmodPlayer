#include "nodes/fmod_geometry_instance_3d.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>
#include <godot_cpp/classes/concave_polygon_shape3d.hpp>
#include <godot_cpp/core/memory.hpp>

namespace godot {
	void FmodGeometryInstance3D::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_direct_occlusion", "value"), &FmodGeometryInstance3D::set_direct_occlusion);
		ClassDB::bind_method(D_METHOD("get_direct_occlusion"), &FmodGeometryInstance3D::get_direct_occlusion);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "direct_occlusion", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_direct_occlusion", "get_direct_occlusion");

		ClassDB::bind_method(D_METHOD("set_reverb_occlusion", "value"), &FmodGeometryInstance3D::set_reverb_occlusion);
		ClassDB::bind_method(D_METHOD("get_reverb_occlusion"), &FmodGeometryInstance3D::get_reverb_occlusion);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "reverb_occlusion", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_reverb_occlusion", "get_reverb_occlusion");

		ClassDB::bind_method(D_METHOD("set_double_sided", "value"), &FmodGeometryInstance3D::set_double_sided);
		ClassDB::bind_method(D_METHOD("get_double_sided"), &FmodGeometryInstance3D::get_double_sided);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "double_sided"), "set_double_sided", "get_double_sided");

		ClassDB::bind_method(D_METHOD("set_active", "value"), &FmodGeometryInstance3D::set_active);
		ClassDB::bind_method(D_METHOD("get_active"), &FmodGeometryInstance3D::get_active);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");

		ClassDB::bind_method(D_METHOD("set_auto_rebuild", "value"), &FmodGeometryInstance3D::set_auto_rebuild);
		ClassDB::bind_method(D_METHOD("get_auto_rebuild"), &FmodGeometryInstance3D::get_auto_rebuild);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_rebuild"), "set_auto_rebuild", "get_auto_rebuild");

		ClassDB::bind_method(D_METHOD("set_show_debug_gizmo", "value"), &FmodGeometryInstance3D::set_show_debug_gizmo);
		ClassDB::bind_method(D_METHOD("get_show_debug_gizmo"), &FmodGeometryInstance3D::get_show_debug_gizmo);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "show_debug_gizmo"), "set_show_debug_gizmo", "get_show_debug_gizmo");

		ClassDB::bind_method(D_METHOD("rebuild_geometry"), &FmodGeometryInstance3D::rebuild_geometry);
		ClassDB::bind_method(D_METHOD("clear_geometry"), &FmodGeometryInstance3D::clear_geometry);
		ClassDB::bind_method(D_METHOD("has_valid_geometry"), &FmodGeometryInstance3D::has_valid_geometry);
		ClassDB::bind_method(D_METHOD("get_geometry"), &FmodGeometryInstance3D::get_geometry);

		ADD_SIGNAL(MethodInfo("geometry_created"));
		ADD_SIGNAL(MethodInfo("geometry_cleared"));
		ADD_SIGNAL(MethodInfo("geometry_rebuilt"));
	}

	void FmodGeometryInstance3D::_notification(int p_what) {
		switch (p_what) {
			case NOTIFICATION_ENTER_TREE:
				_enter_tree();
				break;
			case NOTIFICATION_READY:
				_ready();
				break;
			case NOTIFICATION_EXIT_TREE:
				_exit_tree();
				break;
			case NOTIFICATION_PROCESS:
				_process(get_process_delta_time());
				break;
			#ifdef TOOLS_ENABLED
			case NOTIFICATION_EDITOR_PRE_SAVE:
				// 编辑器保存前，确保几何体已创建
				if (!has_valid_geometry() && auto_rebuild) {
					rebuild_geometry();
				}
				break;
			#endif
		}
	}

	FmodGeometryInstance3D::FmodGeometryInstance3D() {
		// 默认设置为处理（用于网格变化检测）
		set_process(true);
	}

	FmodGeometryInstance3D::~FmodGeometryInstance3D() {
		clear_geometry();
	}

	void FmodGeometryInstance3D::_enter_tree() {
		detect_source_node();
	}

	void FmodGeometryInstance3D::_ready() {
		if (auto_rebuild) {
			rebuild_geometry();
		}
	}

	void FmodGeometryInstance3D::_exit_tree() {
		clear_geometry();
	}

	void FmodGeometryInstance3D::_process(double delta) {
		#ifdef TOOLS_ENABLED
		// 编辑器模式下检测网格变化
		if (Engine::get_singleton()->is_editor_hint() && auto_rebuild && has_valid_geometry()) {
			if (cached_mesh_instance && cached_mesh_instance->get_mesh().is_valid()) {
				Ref<Mesh> mesh = cached_mesh_instance->get_mesh();
				uint32_t current_version = mesh->get_rid().get_id();
				if (current_version != last_mesh_version) {
					last_mesh_version = current_version;
					rebuild_geometry();
				}
			}
		}
		#endif
	}

	void FmodGeometryInstance3D::detect_source_node() {
		Node* parent = get_parent();
		
		if (is_valid_source_node(parent)) {
			if (parent->is_class("MeshInstance3D")) {
				cached_mesh_instance = Object::cast_to<MeshInstance3D>(parent);
				cached_collision_shape = nullptr;
			} else if (parent->is_class("CollisionShape3D")) {
				cached_collision_shape = Object::cast_to<CollisionShape3D>(parent);
				cached_mesh_instance = nullptr;
			}
		} else {
			// 尝试查找祖父节点（如果是嵌套的）
			Node* grandparent = parent ? parent->get_parent() : nullptr;
			if (is_valid_source_node(grandparent)) {
				if (grandparent->is_class("MeshInstance3D")) {
					cached_mesh_instance = Object::cast_to<MeshInstance3D>(grandparent);
					cached_collision_shape = nullptr;
				} else if (grandparent->is_class("CollisionShape3D")) {
					cached_collision_shape = Object::cast_to<CollisionShape3D>(grandparent);
					cached_mesh_instance = nullptr;
				}
			}
		}
	}

	bool FmodGeometryInstance3D::is_valid_source_node(Node* node) const {
		if (!node) return false;
		return node->is_class("MeshInstance3D") || node->is_class("CollisionShape3D");
	}

	void FmodGeometryInstance3D::rebuild_geometry() {
		clear_geometry();
		
		bool success = false;
		
		if (cached_mesh_instance) {
			success = build_from_mesh_instance(cached_mesh_instance);
		} else if (cached_collision_shape) {
			success = build_from_collision_shape(cached_collision_shape);
		} else {
			// 重新检测
			detect_source_node();
			if (cached_mesh_instance) {
				success = build_from_mesh_instance(cached_mesh_instance);
			} else if (cached_collision_shape) {
				success = build_from_collision_shape(cached_collision_shape);
			}
		}
		
		if (success) {
			emit_signal("geometry_rebuilt");
			emit_signal("geometry_created");
		}
	}

	void FmodGeometryInstance3D::clear_geometry() {
		if (geometry.is_valid()) {
			geometry->release();
			geometry.unref();
			emit_signal("geometry_cleared");
		}
	}

	bool FmodGeometryInstance3D::has_valid_geometry() const {
		return geometry.is_valid() && geometry->geometry_is_valid();
	}

	bool FmodGeometryInstance3D::build_from_mesh_instance(MeshInstance3D* mesh_instance) {
		ERR_FAIL_COND_V(!mesh_instance, false);
		
		Ref<Mesh> mesh = mesh_instance->get_mesh();
		if (!mesh.is_valid()) {
			UtilityFunctions::push_warning("MeshInstance3D has no mesh: " + mesh_instance->get_name());
			return false;
		}
		
		Transform3D world_transform = mesh_instance->get_global_transform();
		return build_from_mesh(mesh, world_transform);
	}

	bool FmodGeometryInstance3D::build_from_collision_shape(CollisionShape3D* collision_shape) {
		ERR_FAIL_COND_V(!collision_shape, false);
		
		Ref<Shape3D> shape = collision_shape->get_shape();
		if (!shape.is_valid()) {
			UtilityFunctions::push_warning("CollisionShape3D has no shape: " + collision_shape->get_name());
			return false;
		}
		
		// TODO: 实现从 Shape3D 创建几何体的逻辑
		// 暂时不支持，返回 false
		UtilityFunctions::push_warning("Building geometry from CollisionShape3D is not yet fully implemented");
		return false;
	}

	bool FmodGeometryInstance3D::build_from_mesh(Ref<Mesh> mesh, const Transform3D& transform) {
		ERR_FAIL_COND_V(!mesh.is_valid(), false);
		
		FmodSystem* system = FmodServer::get_singleton()->get_main_system();
		ERR_FAIL_COND_V(!system, false);
		
		geometry = system->create_geometry();
		ERR_FAIL_COND_V(!geometry.is_valid(), false);
		
		// 获取所有三角形
		PackedVector3Array triangles = get_mesh_triangles(mesh);
		
		if (triangles.size() < 3) {
			UtilityFunctions::push_warning("Mesh has no valid triangles");
			return false;
		}
		
		// 转换坐标并添加到几何体
		int polygon_count = 0;
		for (int i = 0; i < triangles.size(); i += 3) {
			PackedVector3Array poly;
			poly.resize(3);
			
			for (int j = 0; j < 3; j++) {
				Vector3 world_pos = transform.xform(triangles[i + j]);
				poly[j] = godot_to_fmod_vector(world_pos);
			}
			
			int result = geometry->add_polygon(direct_occlusion, reverb_occlusion, double_sided, poly);
			if (result >= 0) {
				polygon_count++;
			}
		}
		
		// 设置激活状态
		geometry->set_active(active);
		
		// 记录网格版本（用于变化检测）
		#ifdef TOOLS_ENABLED
		last_mesh_version = mesh->get_rid().get_id();
		#endif
		
		return true;
	}

	PackedVector3Array FmodGeometryInstance3D::get_mesh_triangles(Ref<Mesh> mesh) {
		PackedVector3Array result;
		
		if (!mesh.is_valid()) return result;
		
		for (int s = 0; s < mesh->get_surface_count(); s++) {
			Array arrays = mesh->surface_get_arrays(s);
			if (arrays.is_empty()) continue;
			
			PackedVector3Array vertices = arrays[Mesh::ARRAY_VERTEX];
			PackedInt32Array indices = arrays[Mesh::ARRAY_INDEX];
			
			if (vertices.is_empty()) continue;
			
			if (indices.is_empty()) {
				// 无索引，假设是三角形列表
				for (int i = 0; i < vertices.size(); i++) {
					result.append(vertices[i]);
				}
			} else {
				// 使用索引
				for (int i = 0; i < indices.size(); i++) {
					result.append(vertices[indices[i]]);
				}
			}
		}
		
		return result;
	}

	Vector3 FmodGeometryInstance3D::godot_to_fmod_vector(const Vector3& v) {
		// FMOD 使用左手坐标系，Godot 使用右手坐标系
		// 翻转 Z 轴
		return Vector3(v.x, v.y, -v.z);
	}

	Vector3 FmodGeometryInstance3D::fmod_to_godot_vector(const Vector3& v) {
		return Vector3(v.x, v.y, -v.z);
	}

	// === 属性设置器/获取器 ===
	void FmodGeometryInstance3D::set_direct_occlusion(float p_value) {
		direct_occlusion = Math::clamp(p_value, 0.0f, 1.0f);
		if (has_valid_geometry()) {
			// 更新所有多边形的属性
			// 注意：这需要遍历所有多边形，可能较耗时
			// 暂时简单处理：重建几何体
			if (auto_rebuild) {
				rebuild_geometry();
			}
		}
	}

	float FmodGeometryInstance3D::get_direct_occlusion() const {
		return direct_occlusion;
	}

	void FmodGeometryInstance3D::set_reverb_occlusion(float p_value) {
		reverb_occlusion = Math::clamp(p_value, 0.0f, 1.0f);
		if (has_valid_geometry() && auto_rebuild) {
			rebuild_geometry();
		}
	}

	float FmodGeometryInstance3D::get_reverb_occlusion() const {
		return reverb_occlusion;
	}

	void FmodGeometryInstance3D::set_double_sided(bool p_value) {
		double_sided = p_value;
		if (has_valid_geometry() && auto_rebuild) {
			rebuild_geometry();
		}
	}

	bool FmodGeometryInstance3D::get_double_sided() const {
		return double_sided;
	}

	void FmodGeometryInstance3D::set_active(bool p_value) {
		active = p_value;
		if (has_valid_geometry()) {
			geometry->set_active(active);
		}
	}

	bool FmodGeometryInstance3D::get_active() const {
		return active;
	}

	void FmodGeometryInstance3D::set_auto_rebuild(bool p_value) {
		auto_rebuild = p_value;
		set_process(auto_rebuild);
	}

	bool FmodGeometryInstance3D::get_auto_rebuild() const {
		return auto_rebuild;
	}

	void FmodGeometryInstance3D::set_show_debug_gizmo(bool p_value) {
		show_debug_gizmo = p_value;
		// 通知编辑器更新 gizmo
		#ifdef TOOLS_ENABLED
		update_gizmos();
		#endif
	}

	bool FmodGeometryInstance3D::get_show_debug_gizmo() const {
		return show_debug_gizmo;
	}

	Ref<FmodGeometry> FmodGeometryInstance3D::get_geometry() const {
		return geometry;
	}
}
