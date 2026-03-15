#ifndef FMOD_GEOMETRY_H
#define FMOD_GEOMETRY_H

#include "core/fmod_utils.hpp"
#include <fmod.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

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

		bool geometry_is_valid() const;									// 检查多边形是否有效
		bool geometry_is_null() const;									// 检查多边形是否无效

		void setup(FMOD::Geometry* p_geometry);	

		// 多边形
		void set_polygon_attributes(
			const int index,
			const float direct_occlusion,
			const float reverb_occlusion,
			const bool double_sided
		);																// 设置多边形属性
		Dictionary get_polygon_attributes(const int index) const;		// 获取多边形属性
		int get_polygon_num_vertices(const int index) const;			// 获取多边形中的顶点数
		void set_polygon_vertex(
			const int index, 
			const int vertex_index, 
			const Vector3 vertex
		);																// 设置多边形顶点位置
		Vector3 get_polygon_vertex(
			const int index, 
			const int vertex_index
		) const;														// 获取多边形顶点位置

		// 物体空间化
		void set_position(const Vector3 position);						// 设置多边形三维位置
		Vector3 get_position() const;									// 获取多边形三维位置

		void set_rotation(const Vector3 rotation);						// 设置多边形三维方向
		Vector3 get_rotation() const;									// 获取多边形三维方向
		
		void set_scale(const Vector3 scale);							// 设置多边形三维比例
		Vector3 get_scale() const;										// 获取多边形三维比例

		// 通用对象
		int add_polygon(
			const float direct_occlusion,
			const float reverb_occlusion,
			const bool double_dided,
			const PackedVector3Array& vertices
		) const;														// 添加一个多边形

		void set_active(const bool active);								// 设置物体是否被几何引擎处理
		bool get_active() const;										// 获取几何引擎是否处理了物体

		Dictionary get_max_polygons() const;							// 获取该对象可分配的最大多边形和顶点数
		int get_num_polygons() const;									// 获取该对象中的多边形数量
		PackedByteArray save() const;									// 将几何对象保存为序列化的二进制块，存储在用户内存缓冲区中
		int get_save_size() const;										// 获取集合对象保存时的数据大小
		void release();													// 释放几何对象并释放其内存
	};
}

#endif // !FMOD_GEOMETRY_H
