#include "editor/fmod_audio_stream_player_3d_gizmo_plugin.h"

#include "nodes/fmod_audio_stream_player_3d.h"
#include <godot_cpp/classes/editor_node3d_gizmo.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/geometry3d.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>

namespace godot {
    FmodAudioStreamPlayer3DGizmoPlugin::FmodAudioStreamPlayer3DGizmoPlugin() {
        // 使用青色作为 FMOD 音频的颜色
        Color gizmo_color = Color(0.2, 0.8, 1.0, 0.6);

        create_icon_material("stream_player_3d_icon", FmodUtils::get_editor_theme_icon("AudioStreamPlayer3D"));
        create_material("stream_player_3d_material_primary", gizmo_color);
        create_material("stream_player_3d_material_secondary", gizmo_color * Color(1, 1, 1, 0.35));
        create_handle_material("handles");
    }

    bool FmodAudioStreamPlayer3DGizmoPlugin::_has_gizmo(Node3D* p_for_node_3d) const {
        return Object::cast_to<FmodAudioStreamPlayer3D>(p_for_node_3d) != nullptr;
    }

    String FmodAudioStreamPlayer3DGizmoPlugin::_get_gizmo_name() const {
        return "FmodAudioStreamPlayer3D";
    }

    int32_t FmodAudioStreamPlayer3DGizmoPlugin::_get_priority() const {
        return -1;
    }

    void FmodAudioStreamPlayer3DGizmoPlugin::_redraw(const Ref<EditorNode3DGizmo>& p_gizmo) {
        p_gizmo->clear();

        FmodAudioStreamPlayer3D* player = Object::cast_to<FmodAudioStreamPlayer3D>(p_gizmo->get_node_3d());
        if (!player) return;

        float min_dist = player->get_min_distance();
        float max_dist = player->get_max_distance();

        Ref<StandardMaterial3D> inner_mat = get_material("stream_player_3d_material_primary", p_gizmo);
        Ref<StandardMaterial3D> outer_mat = get_material("stream_player_3d_material_secondary", p_gizmo);

        // 绘制 min_distance 球体线框（经线和纬线）
        if (min_dist > 0.01f) {
            PackedVector3Array lines;
            int rings = 6;      // 纬线数量
            int segments = 12;  // 经线分段数

            // 绘制纬线（水平圆环）
            for (int i = 1; i < rings; i++) {
                float lat = Math_PI * ((float)i / rings - 0.5f);  // -PI/2 到 PI/2
                float y = Math::sin(lat) * min_dist;
                float radius = Math::cos(lat) * min_dist;

                for (int j = 0; j < segments; j++) {
                    float lng1 = 2 * Math_PI * (float)j / segments;
                    float lng2 = 2 * Math_PI * (float)(j + 1) / segments;

                    Vector3 p1(radius * Math::cos(lng1), y, radius * Math::sin(lng1));
                    Vector3 p2(radius * Math::cos(lng2), y, radius * Math::sin(lng2));

                    lines.push_back(p1);
                    lines.push_back(p2);
                }
            }

            // 绘制经线（垂直弧线）
            for (int i = 0; i < segments; i++) {
                float lng = 2 * Math_PI * (float)i / segments;

                for (int j = 0; j < rings; j++) {
                    float lat1 = Math_PI * ((float)j / rings - 0.5f);
                    float lat2 = Math_PI * ((float)(j + 1) / rings - 0.5f);

                    Vector3 p1(
                        min_dist * Math::cos(lat1) * Math::cos(lng),
                        min_dist * Math::sin(lat1),
                        min_dist * Math::cos(lat1) * Math::sin(lng)
                    );
                    Vector3 p2(
                        min_dist * Math::cos(lat2) * Math::cos(lng),
                        min_dist * Math::sin(lat2),
                        min_dist * Math::cos(lat2) * Math::sin(lng)
                    );

                    lines.push_back(p1);
                    lines.push_back(p2);
                }
            }

            if (lines.size() > 0) {
                p_gizmo->add_lines(lines, inner_mat);
            }
        }

        // 绘制 max_distance 球体线框（简化版本）
        if (max_dist > min_dist + 0.01f) {
            PackedVector3Array lines;
            int rings = 4;
            int segments = 8;

            // 只绘制赤道和两极线，更简洁
            for (int i = 0; i < rings; i++) {
                float lat = Math_PI * ((float)i / (rings - 1) - 0.5f);
                float y = Math::sin(lat) * max_dist;
                float radius = Math::cos(lat) * max_dist;

                for (int j = 0; j < segments; j++) {
                    float lng1 = 2 * Math_PI * (float)j / segments;
                    float lng2 = 2 * Math_PI * (float)(j + 1) / segments;

                    Vector3 p1(radius * Math::cos(lng1), y, radius * Math::sin(lng1));
                    Vector3 p2(radius * Math::cos(lng2), y, radius * Math::sin(lng2));

                    lines.push_back(p1);
                    lines.push_back(p2);
                }
            }

            // 经线
            for (int i = 0; i < segments; i += 2) {  // 每隔一条画一条，更稀疏
                float lng = 2 * Math_PI * (float)i / segments;
                for (int j = 0; j < rings - 1; j++) {
                    float lat1 = Math_PI * ((float)j / (rings - 1) - 0.5f);
                    float lat2 = Math_PI * ((float)(j + 1) / (rings - 1) - 0.5f);

                    Vector3 p1(
                        max_dist * Math::cos(lat1) * Math::cos(lng),
                        max_dist * Math::sin(lat1),
                        max_dist * Math::cos(lat1) * Math::sin(lng)
                    );
                    Vector3 p2(
                        max_dist * Math::cos(lat2) * Math::cos(lng),
                        max_dist * Math::sin(lat2),
                        max_dist * Math::cos(lat2) * Math::sin(lng)
                    );

                    lines.push_back(p1);
                    lines.push_back(p2);
                }
            }

            if (lines.size() > 0) {
                p_gizmo->add_lines(lines, outer_mat);
            }
        }
    }
}
