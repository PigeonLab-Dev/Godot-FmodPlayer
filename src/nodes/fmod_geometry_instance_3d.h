#ifndef FMOD_GEOMETRY_INSTANCE_3D_H
#define FMOD_GEOMETRY_INSTANCE_3D_H

#include "geometry/fmod_geometry.h"

#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <vector>

namespace godot {
	class FmodGeometryInstance3D : public StaticBody3D {
		GDCLASS(FmodGeometryInstance3D, StaticBody3D)

	public:
		enum SourceMode {
			SOURCE_AUTO = 0,
			SOURCE_COLLISION_SHAPES = 1,
			SOURCE_MESH_INSTANCE = 2,
			SOURCE_MESH_RESOURCE = 3,
		};

	private:
		Ref<FmodGeometry> geometry;

		SourceMode source_mode = SOURCE_COLLISION_SHAPES;
		NodePath source_node_path;
		Ref<Mesh> mesh;

		float direct_occlusion = 0.5f;
		float reverb_occlusion = 0.5f;
		bool double_sided = true;
		bool active = true;
		bool auto_rebuild = true;
		bool sync_transform = true;
		bool recursive_source_scan = true;
		bool show_debug_gizmo = true;
		int primitive_segments = 16;

		int polygon_count = 0;
		int vertex_count = 0;

		void _collect_collision_shapes(Node *p_root, std::vector<CollisionShape3D *> &r_shapes) const;
		bool _build_from_collision_shapes(Node *p_root, std::vector<PackedVector3Array> &r_polygons) const;
		bool _build_from_collision_shape(CollisionShape3D *p_collision_shape, std::vector<PackedVector3Array> &r_polygons) const;
		bool _build_from_mesh_instance(MeshInstance3D *p_mesh_instance, std::vector<PackedVector3Array> &r_polygons) const;
		bool _build_from_mesh(const Ref<Mesh> &p_mesh, const Transform3D &p_to_geometry_local, std::vector<PackedVector3Array> &r_polygons) const;
		bool _build_from_shape(const Ref<Shape3D> &p_shape, const Transform3D &p_to_geometry_local, std::vector<PackedVector3Array> &r_polygons) const;

		void _append_box(const Vector3 &p_size, const Transform3D &p_xform, std::vector<PackedVector3Array> &r_polygons) const;
		void _append_sphere(float p_radius, const Transform3D &p_xform, std::vector<PackedVector3Array> &r_polygons) const;
		void _append_cylinder(float p_radius, float p_height, const Transform3D &p_xform, std::vector<PackedVector3Array> &r_polygons) const;
		void _append_triangle(const Vector3 &p_a, const Vector3 &p_b, const Vector3 &p_c, const Transform3D &p_xform, std::vector<PackedVector3Array> &r_polygons) const;

		Node *_get_configured_source_node() const;
		void _rebuild_if_live();

	protected:
		static void _bind_methods();
		void _notification(int p_what);

	public:
		FmodGeometryInstance3D();
		~FmodGeometryInstance3D();

		void rebuild_geometry();
		void clear_geometry();
		bool has_valid_geometry() const;
		void sync_geometry_transform();

		static PackedVector3Array get_mesh_triangles(Ref<Mesh> p_mesh);

		void set_source_mode(SourceMode p_mode);
		SourceMode get_source_mode() const;

		void set_source_node_path(const NodePath &p_path);
		NodePath get_source_node_path() const;

		void set_mesh(const Ref<Mesh> &p_mesh);
		Ref<Mesh> get_mesh() const;

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

		void set_sync_transform(bool p_value);
		bool get_sync_transform() const;

		void set_recursive_source_scan(bool p_value);
		bool get_recursive_source_scan() const;

		void set_primitive_segments(int p_segments);
		int get_primitive_segments() const;

		void set_show_debug_gizmo(bool p_value);
		bool get_show_debug_gizmo() const;

		int get_polygon_count() const;
		int get_vertex_count() const;
		Ref<FmodGeometry> get_geometry() const;
	};
}

VARIANT_ENUM_CAST(FmodGeometryInstance3D::SourceMode);

#endif // FMOD_GEOMETRY_INSTANCE_3D_H
