#include "geometry/fmod_geometry.h"

namespace godot {
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
	
		ClassDB::bind_method(D_METHOD("add_polygon", "direct_occlusion", "reverb_occlusion", "double_dided", "vertices"), &FmodGeometry::add_polygon);
		
		ClassDB::bind_method(D_METHOD("set_active", "active"), &FmodGeometry::set_active);
		ClassDB::bind_method(D_METHOD("get_active"), &FmodGeometry::get_scale);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");

		ClassDB::bind_method(D_METHOD("get_max_polygons"), &FmodGeometry::get_max_polygons);
		ClassDB::bind_method(D_METHOD("save"), &FmodGeometry::save);
		ClassDB::bind_method(D_METHOD("get_save_size"), &FmodGeometry::get_save_size);
		ClassDB::bind_method(D_METHOD("release"), &FmodGeometry::release);
	}

	FmodGeometry::FmodGeometry() {

	}

	FmodGeometry::~FmodGeometry() {
		if (geometry) {
			geometry->setUserData(nullptr);
		}
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
			geometry->setUserData(nullptr);
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
		FMOD_ERR_CHECK(geometry->setPolygonAttributes(index, direct_occlusion, reverb_occlusion, double_sided));
	}

	Dictionary FmodGeometry::get_polygon_attributes(const int index) const {
		ERR_FAIL_COND_V(!geometry, Dictionary());
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
		int numvertices = 0;
		FMOD_ERR_CHECK_V(geometry->getPolygonNumVertices(index, &numvertices), 0);
		return numvertices;
	}

	void FmodGeometry::set_polygon_vertex(const int index, const int vertex_index, const Vector3 vertex) {
		ERR_FAIL_COND(!geometry);
		FMOD_VECTOR fmod_vector = { vertex.x, vertex.y, vertex.z };
		FMOD_ERR_CHECK(geometry->setPolygonVertex(index, vertex_index, &fmod_vector));
	}

	Vector3 FmodGeometry::get_polygon_vertex(const int index, const int vertex_index) const {
		ERR_FAIL_COND_V(!geometry, Vector3());
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
		FMOD_VECTOR fmod_pos = { position.x, position.y, position.z };
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
		FMOD_VECTOR fmod_scale = { scale.x, scale.y, scale.z };
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

	int FmodGeometry::add_polygon(
		const float direct_occlusion,
		const float reverb_occlusion,
		const bool double_dided,
		const PackedVector3Array& vertices
	) const {
		ERR_FAIL_COND_V(!geometry, -1);
		ERR_FAIL_COND_V(vertices.size() < 3, -1);
		
		// 转换 Godot 顶点数组为 FMOD 顶点数组
		LocalVector<FMOD_VECTOR> fmod_vertices;
		fmod_vertices.resize(vertices.size());
		
		for (int i = 0; i < vertices.size(); i++) {
			fmod_vertices[i] = { vertices[i].x, vertices[i].y, vertices[i].z };
		}
		
		int polygonindex = -1;
		FMOD_ERR_CHECK_V(geometry->addPolygon(
			direct_occlusion,
			reverb_occlusion,
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
		int maxpolyhons = 0, maxvertices = 0;
		FMOD_ERR_CHECK_V(geometry->getMaxPolygons(&maxpolyhons, &maxvertices), Dictionary());
		Dictionary result;
		result["max_polyhons"] = maxpolyhons;
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
		geometry->setUserData(nullptr);
		FMOD_RESULT result = geometry->release();
		if (result != FMOD_OK) {
			ERR_PRINT("Failed to release Geometry");
			return;
		}
		geometry = nullptr;
	}
}