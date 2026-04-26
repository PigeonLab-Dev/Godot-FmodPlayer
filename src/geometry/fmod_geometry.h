#ifndef FMOD_GEOMETRY_H
#define FMOD_GEOMETRY_H

#include "core/fmod_utils.hpp"

#include <fmod.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/transform3d.hpp>

namespace godot {
	class FmodGeometry : public RefCounted {
		GDCLASS(FmodGeometry, RefCounted)

	private:
		FMOD::Geometry* geometry = nullptr;

	protected:
		static void _bind_methods();

	public:
		FmodGeometry();
		~FmodGeometry();

		bool geometry_is_valid() const;
		bool geometry_is_null() const;

		void setup(FMOD::Geometry* p_geometry);

		void set_polygon_attributes(
			const int index,
			const float direct_occlusion,
			const float reverb_occlusion,
			const bool double_sided
		);
		Dictionary get_polygon_attributes(const int index) const;
		int get_polygon_num_vertices(const int index) const;
		void set_polygon_vertex(
			const int index,
			const int vertex_index,
			const Vector3 vertex
		);
		Vector3 get_polygon_vertex(
			const int index,
			const int vertex_index
		) const;

		void set_position(const Vector3 position);
		Vector3 get_position() const;

		void set_rotation(const Vector3 rotation);
		Vector3 get_rotation() const;

		void set_scale(const Vector3 scale);
		Vector3 get_scale() const;

		void set_transform(const Transform3D& transform);

		int add_polygon(
			const float direct_occlusion,
			const float reverb_occlusion,
			const bool double_sided,
			const PackedVector3Array& vertices
		) const;

		void set_active(const bool active);
		bool get_active() const;

		Dictionary get_max_polygons() const;
		int get_num_polygons() const;
		PackedByteArray save() const;
		int get_save_size() const;
		void release();
	};
}

#endif // FMOD_GEOMETRY_H
