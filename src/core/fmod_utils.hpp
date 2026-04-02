#ifndef UTILS_TOOLKIT_H
#define UTILS_TOOLKIT_H

#include "fmod_system.h"
#include <algorithm>
#include <cmath>
#include <fmod_errors.h>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/theme.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/translation_server.hpp>
#include <godot_cpp/classes/translation_domain.hpp>

#define FMOD_ERR_CHECK(result)                                                  \
    do {                                                                        \
        FMOD_RESULT _r = (result);                                              \
        if (_r != FMOD_OK) {                                                    \
            godot::UtilityFunctions::push_error(                                \
                "FMOD Error at ", __FUNCTION__, " ", __FILE__, ":", __LINE__,   \
                " - ", FMOD_ErrorString(_r)                                     \
            );                                                                  \
        }                                                                       \
    } while(0)

#define FMOD_ERR_CHECK_V(fmod_call, retval)                                     \
    do {                                                                        \
        FMOD_RESULT _r = (fmod_call);                                           \
        if (_r != FMOD_OK) {                                                    \
            godot::UtilityFunctions::push_error(                                \
                "FMOD Error at ", __FUNCTION__, " ", __FILE__, ":", __LINE__,   \
                " - ", FMOD_ErrorString(_r)                                     \
            );                                                                  \
            return retval;                                                      \
        }                                                                       \
    } while(0)

namespace FmodUtils {
    inline static constexpr double MIN_DB = -60.0;         // 静音阈值
    inline static constexpr double MIN_LINEAR = 0.001;     // 10^(-60/20)

    // 线性值转 dB
    // 输入: 0.0 ~ +∞, 输出: -144.0 dB ~ +∞ dB（0 返回 -144dB 而非 -inf）
    inline double linear_to_db(double linear) {
        // 防止 log(0) 和负数
        linear = std::max(linear, MIN_LINEAR);
        return 20.0 * std::log10(linear);
    }

    // dB 转线性值
    // 输入: -∞ ~ +∞, 输出: 0.0 ~ +∞（-144dB 以下返回 0）
    inline double db_to_linear(double db) {
        // 防止下溢
        if (db <= MIN_DB) return 0.0;
        return std::pow(10.0, db / 20.0);
    }

    // 0.0~1.0 → -60dB~0dB（0 映射到 -inf 显示为 -60dB）
    inline double fader_to_db(double fader) {
        if (fader <= 0.0) return MIN_DB;
        // 使用曲线让 0.5 对应 -20dB 左右（感知线性）
        return 20.0 * std::log10(fader);
    }

    // 采样值转 dBFS（Full Scale）
    // 1.0 = 0 dBFS, 0.5 = -6 dBFS, 0.0 = -inf
    inline double sample_to_dbfs(double sample) {
        double absSample = std::abs(sample);
        if (absSample < 1.0e-10) return -144.0;     // 噪声底
        return 20.0 * std::log10(absSample);
    }

    // dBFS 转采样值（带极性）
    inline double dbfs_to_sample(double dbfs, bool positive = true) {
        double linear = std::pow(10.0, dbfs / 20.0);
        return positive ? linear : -linear;
    }

    // 生成标准GUID格式: {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
    inline godot::String guid_to_string(const FMOD_GUID& guid) {
        godot::String guid_string = godot::vformat("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
            guid.Data1,
            guid.Data2,
            guid.Data3,
            guid.Data4[0], guid.Data4[1],
            guid.Data4[2], guid.Data4[3],
            guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]
        );
		return guid_string;
    }

    inline godot::Ref<godot::Texture2D> get_editor_theme_icon(const godot::String& name) {
        godot::EditorInterface* editor_interface = godot::EditorInterface::get_singleton();
        if (editor_interface == nullptr) return godot::Ref<godot::Texture2D>();
        godot::Ref<godot::Theme> theme = editor_interface->get_editor_theme();
        if (theme.is_null()) return godot::Ref<godot::Texture2D>();
		return theme->get_icon(name, "EditorIcons");
    }

    inline godot::Color get_editor_theme_color(const godot::String& name, const godot::String& theme_type) {
        godot::EditorInterface* editor_interface = godot::EditorInterface::get_singleton();
        if (editor_interface == nullptr) return godot::Color();
        godot::Ref<godot::Theme> theme = editor_interface->get_editor_theme();
        if (theme.is_null()) return godot::Color();
        return theme->get_color(name, theme_type);
    }

    // 将 FMOD 方向向量转换为 Godot 的旋转矩阵
    inline godot::Basis fmod_vectors_to_godot_basis(const FMOD_VECTOR& forward, const FMOD_VECTOR& up) {
        // FMOD (左手系 Y-up) -> Godot (右手系 Y-up) 转换
        godot::Vector3 f(-forward.x, forward.y, forward.z);   // X 取反
        godot::Vector3 u(-up.x, up.y, up.z);                  // X 取反
        
        godot::Vector3 r = u.cross(f).normalized();
        f.normalize();
        u.normalize();
        
        return godot::Basis(r, u, f);
    }

    inline godot::Vector3 fmod_vectors_to_godot_euler(const FMOD_VECTOR& forward, const FMOD_VECTOR& up) {
        // FMOD (左手系 Y-up) -> Godot (右手系 Y-up) 转换
        // 关键：翻转 X 轴来改变手性
        
        godot::Vector3 f(-forward.x, forward.y, forward.z);   // X 取反
        godot::Vector3 u(-up.x, up.y, up.z);                  // X 取反
        
        // 计算右向量（右手系叉乘：up × forward）
        godot::Vector3 r = u.cross(f).normalized();
        f.normalize();
        u.normalize();
        
        // 构建 Godot Basis（列向量：右、上、前）
        godot::Basis basis(r, u, f);
        
        // 返回欧拉角（YXZ 顺序）
        return basis.get_euler();
    }

    // 将 FMOD 方向向量转换为 Godot 的四元数
    inline godot::Quaternion fmod_vectors_to_godot_quat(const FMOD_VECTOR& forward, const FMOD_VECTOR& up) {
        return godot::Quaternion(fmod_vectors_to_godot_basis(forward, up));
    }

    // 将 Godot 欧拉角转换为 FMOD 方向向量
    inline void godot_euler_to_fmod_vectors(const godot::Vector3& euler, FMOD_VECTOR* out_forward, FMOD_VECTOR* out_up) {
        // Godot (右手系 Y-up) -> FMOD (左手系 Y-up) 转换
        godot::Basis basis = godot::Basis::from_euler(euler);
        
        // 提取轴向量（Godot 右手系）
        godot::Vector3 u = basis.get_column(1);  // 上
        godot::Vector3 f = basis.get_column(2);  // 前
        
        // X 取反转换为左手系
        if (out_forward) {
            out_forward->x = -f.x;
            out_forward->y = f.y;
            out_forward->z = f.z;
        }
        if (out_up) {
            out_up->x = -u.x;
            out_up->y = u.y;
            out_up->z = u.z;
        }
    }

	template <typename T>
	static godot::Ref<T> load(const godot::String& p_path) {
		godot::ResourceLoader* loader = godot::ResourceLoader::get_singleton();
		if (loader == nullptr) return godot::Ref<T>();
		godot::Ref<godot::Resource> res = loader->load(p_path);
		if (res.is_null()) return godot::Ref<T>();
		godot::Ref<T> result = godot::Object::cast_to<T>(res.ptr());
		return result;
	}
}

#endif // !UTILS_TOOLKIT_H
