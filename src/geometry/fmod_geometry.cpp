#include "geometry/fmod_geometry.h"

#include <cmath>

namespace godot {
	namespace {
		bool _is_finite(float p_value) {
			return std::isfinite(p_value);
		}

		bool _is_finite_vector(const Vector3& p_vector) {
			return _is_finite(static_cast<float>(p_vector.x)) &&
					_is_finite(static_cast<float>(p_vector.y)) &&
					_is_finite(static_cast<float>(p_vector.z));
		}

		bool _is_finite_transform(const Transform3D& p_transform) {
			return _is_finite_vector(p_transform.origin) &&
					_is_finite_vector(p_transform.basis.get_column(0)) &&
					_is_finite_vector(p_transform.basis.get_column(1)) &&
					_is_finite_vector(p_transform.basis.get_column(2));
		}
	}

	void FmodGeometry::_bind_methods() {
		ClassDB::bind_method(D_METHOD("geometry_is_valid"), &FmodGeometry::geometry_is_valid);
		ClassDB::bind_method(D_METHOD("geometry_is_null"), &FmodGeometry::geometry_is_null);
		
		ClassDB::bind_method(D_METHOD("set_polygon_attributes", "index", "direct_occlusion", "reverb_occlusion", "double_sided"), &FmodGeometry::set_polygon_attributes);
		ClassDB::bind_method(D_METHOD("get_polygon_attributes", "index"), &FmodGeometry::get_polygon_attributes);
		ClassDB::bind_method(D_METHOD("get_polygon_num_vertices", "index"), &FmodGeometry::get_polygon_num_vertices);
		ClassDB::bind_method(D_METHOD("set_polygon_vertex", "index", "vertex_index", "vertex"), &FmodGeometry::set_polygon_vertex);
		ClassDB::bind_method(D_METHOD("get_polygon_vertex", "index", "vertex_index"), &FmodGeometry::get_polygon_vertex);
		
		ClassDB::bind_method(D_METHOD("set_position", "position"), &FmodGeometry::set_position);
		ClassDB::bind_method(D_METHOD("get_position"), &FmodGeometry::get_position);
		ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "position", PROPERTY_HINT_RANGE, "-99999,99999,or_greater,or_less,hide_control,suffix:m", PROPERTY_USAGE_EDITOR), "set_position", "get_position");

		ClassDB::bind_method(D_METHOD("set_rotation", "rotation"), &FmodGeometry::set_rotation);
		ClassDB::bind_method(D_METHOD("get_rotation"), &FmodGeometry::get_rotation);
		ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "rotation", PROPERTY_HINT_RANGE, "-360,360,0.1,or_less,or_greater,radians_as_degrees", PROPERTY_USAGE_EDITOR), "set_rotation", "get_rotation");

		ClassDB::bind_method(D_METHOD("set_scale", "scale"), &FmodGeometry::set_scale);
		ClassDB::bind_method(D_METHOD("get_scale"), &FmodGeometry::get_scale);
		ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "scale", PROPERTY_HINT_LINK, "", PROPERTY_USAGE_EDITOR), "set_scale", "get_scale");
	
		ClassDB::bind_method(D_METHOD("set_transform", "transform"), &FmodGeometry::set_transform);
		ClassDB::bind_method(D_METHOD("add_polygon", "direct_occlusion", "reverb_occlusion", "double_sided", "vertices"), &FmodGeometry::add_polygon);
		
		ClassDB::bind_method(D_METHOD("set_active", "active"), &FmodGeometry::set_active);
		ClassDB::bind_method(D_METHOD("get_active"), &FmodGeometry::get_active);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");

		ClassDB::bind_method(D_METHOD("get_max_polygons"), &FmodGeometry::get_max_polygons);
		ClassDB::bind_method(D_METHOD("save"), &FmodGeometry::save);
		ClassDB::bind_method(D_METHOD("get_save_size"), &FmodGeometry::get_save_size);
		ClassDB::bind_method(D_METHOD("release"), &FmodGeometry::release);
	}

	FmodGeometry::FmodGeometry() {

	}

	FmodGeometry::~FmodGeometry() {
		_release_internal(false);
	}

	bool FmodGeometry::_release_internal(bool p_restore_userdata_on_failure) {
		if (!geometry) {
			return true;
		}

		FMOD::Geometry* geometry_to_release = geometry;
		geometry_to_release->setUserData(nullptr);
		const FMOD_RESULT result = geometry_to_release->release();
		if (result != FMOD_OK) {
			if (p_restore_userdata_on_failure) {
				geometry_to_release->setUserData(this);
			} else {
				geometry = nullptr;
			}
			ERR_PRINT(vformat("Failed to release Geometry: %s", FMOD_ErrorString(result)));
			return false;
		}

		geometry = nullptr;
		return true;
	}

	bool FmodGeometry::geometry_is_valid() const {
		return geometry != nullptr;
	}

	bool FmodGeometry::geometry_is_null() const {
		return geometry == nullptr;
	}

	void FmodGeometry::setup(FMOD::Geometry* p_geometry) {
		ERR_FAIL_COND_MSG(!p_geometry, "Geometry pointer is null");

		if (geometry) {
			ERR_FAIL_COND_MSG(!_release_internal(true), "Failed to release previous Geometry before setup.");
		}

		geometry = p_geometry;
		geometry->setUserData(this);
	}

	void FmodGeometry::set_polygon_attributes(
		const int index,
		const float direct_occlusion,
		const float reverb_occlusion,
		const bool double_sided
	) {
		ERR_FAIL_COND(!geometry);
		ERR_FAIL_COND_MSG(index < 0, "Polygon index must be >= 0.");
		ERR_FAIL_COND_MSG(!_is_finite(direct_occlusion), "direct_occlusion must be finite.");
		ERR_FAIL_COND_MSG(!_is_finite(reverb_occlusion), "reverb_occlusion must be finite.");
		FMOD_ERR_CHECK(geometry->setPolygonAttributes(index, CLAMP(direct_occlusion, 0.0f, 1.0f), CLAMP(reverb_occlusion, 0.0f, 1.0f), double_sided));
	}

	Dictionary FmodGeometry::get_polygon_attributes(const int index) const {
		ERR_FAIL_COND_V(!geometry, Dictionary());
		ERR_FAIL_COND_V_MSG(index < 0, Dictionary(), "Polygon index must be >= 0.");
		float directocclusion = 0.0f, reverbocclusion = 0.0f;
		bool doublesided = false;
		FMOD_ERR_CHECK_V(geometry->getPolygonAttributes(index, &directocclusion, &reverbocclusion, &doublesided), Dictionary());
		Dictionary result;
		result["direct_occlusion"] = directocclusion;
		result["reverb_occlusion"] = reverbocclusion;
		result["double_sided"] = doublesided;
		return result;
	}

	int FmodGeometry::get_polygon_num_vertices(const int index) const {
		ERR_FAIL_COND_V(!geometry, 0);
		ERR_FAIL_COND_V_MSG(index < 0, 0, "Polygon index must be >= 0.");
		int numvertices = 0;
		FMOD_ERR_CHECK_V(geometry->getPolygonNumVertices(index, &numvertices), 0);
		return numvertices;
	}

	void FmodGeometry::set_polygon_vertex(const int index, const int vertex_index, const Vector3 vertex) {
		ERR_FAIL_COND(!geometry);
		ERR_FAIL_COND_MSG(index < 0, "Polygon index must be >= 0.");
		ERR_FAIL_COND_MSG(vertex_index < 0, "Vertex index must be >= 0.");
		ERR_FAIL_COND_MSG(!_is_finite_vector(vertex), "Vertex must be finite.");
		FMOD_VECTOR fmod_vector = { static_cast<float>(vertex.x), static_cast<float>(vertex.y), static_cast<float>(vertex.z) };
		FMOD_ERR_CHECK(geometry->setPolygonVertex(index, vertex_index, &fmod_vector));
	}

	Vector3 FmodGeometry::get_polygon_vertex(const int index, const int vertex_index) const {
		ERR_FAIL_COND_V(!geometry, Vector3());
		ERR_FAIL_COND_V_MSG(index < 0, Vector3(), "Polygon index must be >= 0.");
		ERR_FAIL_COND_V_MSG(vertex_index < 0, Vector3(), "Vertex index must be >= 0.");
		FMOD_VECTOR fmod_vector = {};
		FMOD_ERR_CHECK_V(geometry->getPolygonVertex(index, vertex_index, &fmod_vector), Vector3());
		Vector3 vertex;
		vertex.x = fmod_vector.x;
		vertex.y = fmod_vector.y;
		vertex.z = fmod_vector.z;
		return vertex;
	}

	void FmodGeometry::set_position(const Vector3 position) {
		ERR_FAIL_COND(!geometry);
		ERR_FAIL_COND_MSG(!_is_finite_vector(position), "Position must be finite.");
		FMOD_VECTOR fmod_pos = { static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z) };
		FMOD_ERR_CHECK(geometry->setPosition(&fmod_pos));
	}

	Vector3 FmodGeometry::get_position() const {
		if (!geometry) return Vector3();
		FMOD_VECTOR fmod_pos = {};
		FMOD_ERR_CHECK_V(geometry->getPosition(&fmod_pos), Vector3());
		Vector3 pos;
		pos.x = fmod_pos.x;
		pos.y = fmod_pos.y;
		pos.z = fmod_pos.z;
		return pos;
	}

	void FmodGeometry::set_rotation(const Vector3 rotation) {
		ERR_FAIL_COND(!geometry);
		ERR_FAIL_COND_MSG(!_is_finite_vector(rotation), "Rotation must be finite.");
		FMOD_VECTOR forward = {}, up = {};
		FmodUtils::godot_euler_to_fmod_vectors(rotation, &forward, &up);
		FMOD_ERR_CHECK(geometry->setRotation(&forward, &up));
	}

	Vector3 FmodGeometry::get_rotation() const {
		if (!geometry) return Vector3();
		FMOD_VECTOR forward = {}, up = {};
		FMOD_ERR_CHECK_V(geometry->getRotation(&forward, &up), Vector3());
		return FmodUtils::fmod_vectors_to_godot_euler(forward, up);
	}

	void FmodGeometry::set_scale(const Vector3 scale) {
		ERR_FAIL_COND(!geometry);
		ERR_FAIL_COND_MSG(!_is_finite_vector(scale), "Scale must be finite.");
		FMOD_VECTOR fmod_scale = { static_cast<float>(scale.x), static_cast<float>(scale.y), static_cast<float>(scale.z) };
		FMOD_ERR_CHECK(geometry->setScale(&fmod_scale));
	}
	
	Vector3 FmodGeometry::get_scale() const {
		if (!geometry) return Vector3();
		FMOD_VECTOR fmod_scale = {};
		FMOD_ERR_CHECK_V(geometry->getScale(&fmod_scale), Vector3());
		Vector3 scale;
		scale.x = fmod_scale.x;
		scale.y = fmod_scale.y;
		scale.z = fmod_scale.z;
		return scale;
	}

	void FmodGeometry::set_transform(const Transform3D& transform) {
		ERR_FAIL_COND(!geometry);
		ERR_FAIL_COND_MSG(!_is_finite_transform(transform), "Transform must be finite.");

		Vector3 position = transform.origin;
		Vector3 scale = transform.basis.get_scale();
		Basis basis = transform.basis.orthonormalized();

		Vector3 forward = basis.get_column(2);
		Vector3 up = basis.get_column(1);
		ERR_FAIL_COND_MSG(!_is_finite_vector(forward) || !_is_finite_vector(up), "Transform basis must contain a valid rotation.");

		FMOD_VECTOR fmod_pos = { static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z) };
		FMOD_VECTOR fmod_scale = { static_cast<float>(scale.x), static_cast<float>(scale.y), static_cast<float>(scale.z) };
		FMOD_VECTOR fmod_forward = { static_cast<float>(forward.x), static_cast<float>(forward.y), static_cast<float>(forward.z) };
		FMOD_VECTOR fmod_up = { static_cast<float>(up.x), static_cast<float>(up.y), static_cast<float>(up.z) };

		FMOD_ERR_CHECK(geometry->setPosition(&fmod_pos));
		FMOD_ERR_CHECK(geometry->setRotation(&fmod_forward, &fmod_up));
		FMOD_ERR_CHECK(geometry->setScale(&fmod_scale));
	}

	int FmodGeometry::add_polygon(
		const float direct_occlusion,
		const float reverb_occlusion,
		const bool double_dided,
		const PackedVector3Array& vertices
	) const {
		ERR_FAIL_COND_V(!geometry, -1);
		ERR_FAIL_COND_V(vertices.size() < 3, -1);
		ERR_FAIL_COND_V_MSG(!_is_finite(direct_occlusion), -1, "direct_occlusion must be finite.");
		ERR_FAIL_COND_V_MSG(!_is_finite(reverb_occlusion), -1, "reverb_occlusion must be finite.");
		
		// 转换 Godot 顶点数组为 FMOD 顶点数组
		LocalVector<FMOD_VECTOR> fmod_vertices;
		fmod_vertices.resize(vertices.size());
		
		for (int i = 0; i < vertices.size(); i++) {
			ERR_FAIL_COND_V_MSG(!_is_finite_vector(vertices[i]), -1, "Polygon vertices must be finite.");
			fmod_vertices[i] = { static_cast<float>(vertices[i].x), static_cast<float>(vertices[i].y), static_cast<float>(vertices[i].z) };
		}
		
		int polygonindex = -1;
		FMOD_ERR_CHECK_V(geometry->addPolygon(
			CLAMP(direct_occlusion, 0.0f, 1.0f),
			CLAMP(reverb_occlusion, 0.0f, 1.0f),
			double_dided,
			vertices.size(),
			fmod_vertices.ptr(),
			&polygonindex
		), -1);
		return polygonindex;
	}

	void FmodGeometry::set_active(const bool active) {
		ERR_FAIL_COND(!geometry);
		FMOD_ERR_CHECK(geometry->setActive(active));
	}

	bool FmodGeometry::get_active() const {
		if (!geometry) return false;
		bool active = false;
		FMOD_ERR_CHECK_V(geometry->getActive(&active), false);
		return active;
	}

	Dictionary FmodGeometry::get_max_polygons() const {
		ERR_FAIL_COND_V(!geometry, Dictionary());
		int maxpolygons = 0, maxvertices = 0;
		FMOD_ERR_CHECK_V(geometry->getMaxPolygons(&maxpolygons, &maxvertices), Dictionary());
		Dictionary result;
		result["max_polygons"] = maxpolygons;
		result["max_vertices"] = maxvertices;
		return result;
	}

	int FmodGeometry::get_num_polygons() const {
		ERR_FAIL_COND_V(!geometry, 0);
		int numpolygons = 0;
		FMOD_ERR_CHECK_V(geometry->getNumPolygons(&numpolygons), 0);
		return numpolygons;
	}

	PackedByteArray FmodGeometry::save() const {
		ERR_FAIL_COND_V(!geometry, PackedByteArray());

		// 获取所需大小
		int size = 0;
		FMOD_ERR_CHECK_V(geometry->save(nullptr, &size), PackedByteArray());
		ERR_FAIL_COND_V_MSG(size <= 0, PackedByteArray(), "Geometry save size must be greater than 0.");

		// 分配缓冲区并保存
		PackedByteArray data;
		data.resize(size);

		FMOD_ERR_CHECK_V(geometry->save(data.ptrw(), &size), PackedByteArray());
		return data;
	}

	int FmodGeometry::get_save_size() const {
		ERR_FAIL_COND_V(!geometry, 0);
		int size = 0;
		FMOD_ERR_CHECK_V(geometry->save(nullptr, &size), 0);
		return size;
	}

	void FmodGeometry::release() {
		ERR_FAIL_COND(!geometry);
		_release_internal(true);
	}
}
