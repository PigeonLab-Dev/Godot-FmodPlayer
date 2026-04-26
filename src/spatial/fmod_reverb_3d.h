#ifndef FMOD_REVERB_3D_H
#define FMOD_REVERB_3D_H

#include "core/fmod_utils.hpp"
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {
	class FmodReverb3D : public RefCounted {
		GDCLASS(FmodReverb3D, RefCounted)

	private:
		FMOD::Reverb3D* reverb_3d = nullptr;

		FMOD_REVERB_PROPERTIES properties = FMOD_PRESET_GENERIC;
		Vector3 position;
		float min_distance = 5.0f;
		float max_distance = 20.0f;
		bool active = true;

		void _apply_properties();
		void _apply_3d_attributes();
		void _apply_active();

	protected:
		static void _bind_methods();

	public:
		FmodReverb3D();
		~FmodReverb3D();

		bool reverb_3d_is_valid() const;								// 检查多边形是否有效
		bool reverb_3d_is_null() const;									// 检查多边形是否无效

		void setup(FMOD::Reverb3D* p_reverb_3d);

		void set_properties(const Dictionary& p_properties);
		Dictionary get_properties() const;

		// 概述
		void set_3d_attributes(
			const Vector3 position,
			const float min_distance,
			const float max_distance
		);																// 设置混响球的三维属性
		Dictionary get_3d_attributes() const;							// 获取混响球的三维属性

		void set_active(const bool active);								// 设置激活状态
		bool get_active() const;										// 获取激活状态

		void release();													// 释放

		// 混响属性设置
		void set_decay_time(const float decay_time);					// 设置混响衰减时间
		float get_decay_time() const;									// 获取混响衰减时间

		void set_early_delay(const float early_delay);					// 设置初始反射延迟时间
		float get_early_delay() const;									// 获取初始反射延迟时间

		void set_late_delay(const float late_delay);					// 设置相对于初始反射的晚期混响延迟时间
		float get_late_delay() const;									// 获取相对于初始反射的晚期混响延迟时间

		void set_hf_reference(const float hf_reference);				// 设置参考高频
		float get_hf_reference() const;									// 获取参考高频

		void set_hf_decay_ratio(const float hf_decay_ratio);			// 设置高频到中频衰减时间比
		float get_hf_decay_ratio() const;								// 获取高频到中频衰减时间比

		void set_diffusion(const float diffusion);						// 设置控制晚期混响衰减回声密度的值
		float get_diffusion() const;									// 获取控制晚期混响衰减回声密度的值

		void set_density(const float density);							// 设置控制晚期混响衰减中的模态密度
		float get_density() const;										// 获取控制晚期混响衰减中的模态密度

		void set_low_shelf_frequency(const float low_shelf_frequency);	// 设置参考低频
		float get_low_shelf_frequency() const;							// 获取参考低频

		void set_low_shelf_gain(const float low_shelf_gain);			// 设置低频反射的相对房间效应电平
		float get_low_shelf_gain() const;								// 获取低频反射的相对房间效应电平

		void set_high_cut(const float high_cut);						// 设置高频反射的相对房间效应电平
		float get_high_cut() const;										// 获取高频反射的相对房间效应电平

		void set_early_late_mix(const float early_late_mix);			// 设置早期反射相对房间效应电平
		float get_early_late_mix() const;								// 获取早期反射相对房间效应电平

		void set_wet_level(const float wet_level);						// 设置中频反射相对房间效应电平
		float get_wet_level() const;									// 获取中频反射相对房间效应电平

	};
}

#endif // !FMOD_REVERB_3D_H
