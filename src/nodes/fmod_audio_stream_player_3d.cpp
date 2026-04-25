#include "nodes/fmod_audio_stream_player_3d.h"
#include "core/fmod_server.h"
#include "mixer/fmod_audio_bus_layout.h"

#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>

namespace godot {
	void FmodAudioStreamPlayer3D::_bind_methods() {
		BIND_ENUM_CONSTANT(ATTENUATION_INVERSE_DISTANCE)
		BIND_ENUM_CONSTANT(ATTENUATION_INVERSE_SQUARE_DISTANCE)
		BIND_ENUM_CONSTANT(ATTENUATION_LOGARITHMIC)
		BIND_ENUM_CONSTANT(ATTENUATION_DISABLED)

		BIND_ENUM_CONSTANT(DOPPLER_TRACKING_DISABLED)
		BIND_ENUM_CONSTANT(DOPPLER_TRACKING_IDLE_STEP)
		BIND_ENUM_CONSTANT(DOPPLER_TRACKING_PHYSICS_STEP)

		ClassDB::bind_method(D_METHOD("play", "from_position"), &FmodAudioStreamPlayer3D::play, DEFVAL(0.0));
		ClassDB::bind_method(D_METHOD("seek", "to_position"), &FmodAudioStreamPlayer3D::seek);
		ClassDB::bind_method(D_METHOD("get_playback_position"), &FmodAudioStreamPlayer3D::get_playback_position);
		ClassDB::bind_method(D_METHOD("stop"), &FmodAudioStreamPlayer3D::stop);
		
		ClassDB::bind_method(D_METHOD("set_stream", "stream"), &FmodAudioStreamPlayer3D::set_stream);
		ClassDB::bind_method(D_METHOD("get_stream"), &FmodAudioStreamPlayer3D::get_stream);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "stream", PROPERTY_HINT_RESOURCE_TYPE, "FmodAudioStream"), "set_stream", "get_stream");

		ClassDB::bind_method(D_METHOD("set_attenuation_model", "model"), &FmodAudioStreamPlayer3D::set_attenuation_model);
		ClassDB::bind_method(D_METHOD("get_attenuation_model"), &FmodAudioStreamPlayer3D::get_attenuation_model);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "attenuation_model", PROPERTY_HINT_ENUM, "Inverse,Inverse Square,Linear (Silent at Max),Disabled"), "set_attenuation_model", "get_attenuation_model");

		ClassDB::bind_method(D_METHOD("set_volume_db", "volume_db"), &FmodAudioStreamPlayer3D::set_volume_db);
		ClassDB::bind_method(D_METHOD("get_volume_db"), &FmodAudioStreamPlayer3D::get_volume_db);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume_db", PROPERTY_HINT_RANGE, "-80,24,0.1,suffix:dB"), "set_volume_db", "get_volume_db");

		ClassDB::bind_method(D_METHOD("set_unit_size", "size"), &FmodAudioStreamPlayer3D::set_unit_size);
		ClassDB::bind_method(D_METHOD("get_unit_size"), &FmodAudioStreamPlayer3D::get_unit_size);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "unit_size", PROPERTY_HINT_RANGE, "0.001,100,0.001,or_greater"), "set_unit_size", "get_unit_size");

		ClassDB::bind_method(D_METHOD("set_pitch_scale", "pitch_scale"), &FmodAudioStreamPlayer3D::set_pitch_scale);
		ClassDB::bind_method(D_METHOD("get_pitch_scale"), &FmodAudioStreamPlayer3D::get_pitch_scale);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch_scale", PROPERTY_HINT_RANGE, "0.01,4,0.01,or_greater"), "set_pitch_scale", "get_pitch_scale");

		ClassDB::bind_method(D_METHOD("set_playing", "playing"), &FmodAudioStreamPlayer3D::set_playing);
		ClassDB::bind_method(D_METHOD("is_playing"), &FmodAudioStreamPlayer3D::is_playing);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "set_playing", "is_playing");

		ClassDB::bind_method(D_METHOD("set_auto_play", "enable"), &FmodAudioStreamPlayer3D::set_auto_play);
		ClassDB::bind_method(D_METHOD("is_autoplay_enabled"), &FmodAudioStreamPlayer3D::is_autoplay_enabled);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_play"), "set_auto_play", "is_autoplay_enabled");

		ClassDB::bind_method(D_METHOD("set_stream_paused", "paused"), &FmodAudioStreamPlayer3D::set_stream_paused);
		ClassDB::bind_method(D_METHOD("get_stream_paused"), &FmodAudioStreamPlayer3D::get_stream_paused);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "stream_paused"), "set_stream_paused", "get_stream_paused");

		ClassDB::bind_method(D_METHOD("set_max_distance", "distance"), &FmodAudioStreamPlayer3D::set_max_distance);
		ClassDB::bind_method(D_METHOD("get_max_distance"), &FmodAudioStreamPlayer3D::get_max_distance);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_distance", PROPERTY_HINT_RANGE, "0.1,1000,0.01,or_greater,suffix:m"), "set_max_distance", "get_max_distance");

		ClassDB::bind_method(D_METHOD("set_bus", "bus"), &FmodAudioStreamPlayer3D::set_bus);
		ClassDB::bind_method(D_METHOD("get_bus"), &FmodAudioStreamPlayer3D::get_bus);
		ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "bus", PROPERTY_HINT_ENUM), "set_bus", "get_bus");

		ClassDB::bind_method(D_METHOD("set_area_mask", "mask"), &FmodAudioStreamPlayer3D::set_area_mask);
		ClassDB::bind_method(D_METHOD("get_area_mask"), &FmodAudioStreamPlayer3D::get_area_mask);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "area_mask", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_area_mask", "get_area_mask");

		ADD_GROUP("Emission Angle", "emission_angle");
		ClassDB::bind_method(D_METHOD("set_emission_angle_enabled", "enabled"), &FmodAudioStreamPlayer3D::set_emission_angle_enabled);
		ClassDB::bind_method(D_METHOD("is_emission_angle_enabled"), &FmodAudioStreamPlayer3D::is_emission_angle_enabled);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "emission_angle_enabled"), "set_emission_angle_enabled", "is_emission_angle_enabled");
		
		ClassDB::bind_method(D_METHOD("set_emission_angle", "angle"), &FmodAudioStreamPlayer3D::set_emission_angle);
		ClassDB::bind_method(D_METHOD("get_emission_angle"), &FmodAudioStreamPlayer3D::get_emission_angle);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "emission_angle", PROPERTY_HINT_RANGE, "0,180,0.1,radians_as_degrees"), "set_emission_angle", "get_emission_angle");
		
		ClassDB::bind_method(D_METHOD("set_emission_angle_filter_attenuation_db", "db"), &FmodAudioStreamPlayer3D::set_emission_angle_filter_attenuation_db);
		ClassDB::bind_method(D_METHOD("get_emission_angle_filter_attenuation_db"), &FmodAudioStreamPlayer3D::get_emission_angle_filter_attenuation_db);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "emission_angle_filter_attenuation_db", PROPERTY_HINT_RANGE, "-80,80,suffix:dB"), "set_emission_angle_filter_attenuation_db", "get_emission_angle_filter_attenuation_db");

		ADD_GROUP("Attenuation Filter", "attenuation_filter_");
		ClassDB::bind_method(D_METHOD("set_attenuation_filter_cutoff_hz", "freq"), &FmodAudioStreamPlayer3D::set_attenuation_filter_cutoff_hz);
		ClassDB::bind_method(D_METHOD("get_attenuation_filter_cutoff_hz"), &FmodAudioStreamPlayer3D::get_attenuation_filter_cutoff_hz);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attenuation_filter_cutoff_hz", PROPERTY_HINT_RANGE, "10,22050,1,suffix:Hz"), "set_attenuation_filter_cutoff_hz", "get_attenuation_filter_cutoff_hz");

		ClassDB::bind_method(D_METHOD("set_attenuation_filter_db", "db"), &FmodAudioStreamPlayer3D::set_attenuation_filter_db);
		ClassDB::bind_method(D_METHOD("get_attenuation_filter_db"), &FmodAudioStreamPlayer3D::get_attenuation_filter_db);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attenuation_filter_db", PROPERTY_HINT_RANGE, "-80,0,0.1,suffix:dB"), "set_attenuation_filter_db", "get_attenuation_filter_db");

		ADD_GROUP("Doppler", "doppler_");
		ClassDB::bind_method(D_METHOD("set_doppler_tracking", "tracking"), &FmodAudioStreamPlayer3D::set_doppler_tracking);
		ClassDB::bind_method(D_METHOD("get_doppler_tracking"), &FmodAudioStreamPlayer3D::get_doppler_tracking);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "doppler_tracking", PROPERTY_HINT_ENUM, "Disabled,Idle Step,Physics Step"), "set_doppler_tracking", "get_doppler_tracking");

		ADD_SIGNAL(MethodInfo("finished"));
	}

	FmodAudioStreamPlayer3D::FmodAudioStreamPlayer3D() {
		internal_channel.instantiate();
		if (FmodServer::get_singleton()) {
			Ref<FmodSystem> system = FmodServer::get_main_system();
			if (system.is_valid()) {
				internal_channel_group = system->get_master_channel_group();
			}
		}
		// 启用处理以便更新 3D 属性
		set_process(true);
		set_physics_process(true);
	}

	FmodAudioStreamPlayer3D::~FmodAudioStreamPlayer3D() {
		if (internal_channel.is_valid()) {
			internal_channel->stop();
		}
	}

	void FmodAudioStreamPlayer3D::_notification(int p_what) {
		switch (p_what) {
			case NOTIFICATION_READY: {
				if (auto_play && stream.is_valid() && !Engine::get_singleton()->is_editor_hint()) {
					call_deferred("play");
				}
				last_position = get_global_position();
				last_process_time = Time::get_singleton()->get_ticks_usec();
			} break;

			case NOTIFICATION_PROCESS: {
				if (!is_inside_tree()) {
					break;
				}
				if (doppler_tracking == DOPPLER_TRACKING_IDLE_STEP) {
					_update_velocity(get_process_delta_time());
				}
				if (internal_channel.is_valid() && internal_channel->is_playing()) {
					_update_3d_attributes();
				}
			} break;

			case NOTIFICATION_PHYSICS_PROCESS: {
				if (!is_inside_tree()) {
					break;
				}
				if (doppler_tracking == DOPPLER_TRACKING_PHYSICS_STEP) {
					_update_velocity(get_physics_process_delta_time());
				}
			} break;

			case NOTIFICATION_EXIT_TREE: {
				if (internal_channel.is_valid()) {
					internal_channel->stop();
					internal_channel.unref();
				}
				if (internal_channel_group.is_valid()) {
					internal_channel_group.unref();
				}
			} break;
		}
	}

	void FmodAudioStreamPlayer3D::_validate_property(PropertyInfo& p_property) const {
		if (p_property.name == StringName("bus")) {
			String bus_list;
			AudioServer* audio_server = AudioServer::get_singleton();
			if (audio_server) {
				int bus_count = audio_server->get_bus_count();
				for (int i = 0; i < bus_count; i++) {
					if (i > 0) bus_list += ",";
					bus_list += audio_server->get_bus_name(i);
				}
			}
			p_property.hint = PROPERTY_HINT_ENUM;
			p_property.hint_string = bus_list;
		}
	}

	void FmodAudioStreamPlayer3D::_create_internal_channel(Ref<FmodAudioStream> p_stream) {
		Ref<FmodSystem> system = FmodServer::get_main_system();
		if (system.is_null()) {
			ERR_PRINT("FMOD system not available");
			return;
		}

		Ref<FmodSound> sound = p_stream->get_sound();
		if (sound.is_null()) {
			ERR_PRINT("Failed to get sound from stream");
			return;
		}

		// 使用暂停模式创建，以便设置 3D 属性
		internal_channel = system->play_sound(sound, internal_channel_group, true);
		if (internal_channel.is_null()) {
			ERR_PRINT("Failed to create FMOD channel");
			return;
		}

		internal_channel->set_volume_db(volume_db);
		internal_channel->set_pitch(pitch);
		
		// 设置为 3D 模式
		internal_channel->set_mode(FmodSystem::FMOD_MODE_3D);
		
		// 设置多普勒级别（完全禁用或启用）
		internal_channel->set_3d_doppler_level(
			doppler_tracking == DOPPLER_TRACKING_DISABLED ? 0.0f : 1.0f
		);
		
		// 应用衰减设置
		_apply_attenuation_settings();
		
		// 设置初始 3D 属性
		_update_3d_attributes();

		internal_channel->connect("ended", callable_mp(this, &FmodAudioStreamPlayer3D::_on_internal_channel_ended), CONNECT_DEFERRED);
	}

	void FmodAudioStreamPlayer3D::_on_internal_channel_ended() {
		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			internal_channel->disconnect("ended", callable_mp(this, &FmodAudioStreamPlayer3D::_on_internal_channel_ended));
		}
		internal_channel.unref();

		// stop 会提前设置 playing 为 false，所以判断是否还在播放就行
		if (playing) {
			playing = false;
			emit_signal("finished");
		}
	}

	void FmodAudioStreamPlayer3D::_update_3d_attributes() {
		if (!internal_channel.is_valid() || !internal_channel->channel_control_is_valid() || !is_inside_tree()) {
			return;
		}

		// 设置 3D 属性（位置和速度）
		// 如果禁用了多普勒追踪，传入零速度避免 pitch 变化
		Vector3 velocity = (doppler_tracking == DOPPLER_TRACKING_DISABLED) ? Vector3(0, 0, 0) : current_velocity;
		internal_channel->set_3d_attributes(get_global_position(), velocity);
		
		// 更新声音锥（如果启用）
		if (emission_angle_enabled) {
			internal_channel->set_3d_cone_settings(
				emission_angle,
				emission_angle * 2.0f,										// 外锥角为内锥的两倍
				emission_angle_filter_attenuation_db
			);
		} else {
			// 禁用声音锥（360度）
			internal_channel->set_3d_cone_settings(360.0f, 360.0f, 0.0f);
		}
	}

	void FmodAudioStreamPlayer3D::_update_velocity(double delta) {
		if (delta <= 0.0 || !is_inside_tree()) {
			return;
		}

		Vector3 current_pos = get_global_position();
		current_velocity = (current_pos - last_position) / delta;
		last_position = current_pos;
	}

	void FmodAudioStreamPlayer3D::_apply_attenuation_settings() {
		// 检查底层 FMOD ChannelControl 是否有效
		if (!internal_channel.is_valid() || !internal_channel->channel_control_is_valid()) {
			return;
		}

		// FMOD 使用 min_distance 和 max_distance 控制衰减
		// 从 unit_size 开始衰减，到 max_distance * unit_size 结束
		float fmod_min_dist = unit_size;
		float fmod_max_dist = max_distance * unit_size;

		// 分开设置最小和最大距离
		internal_channel->set_3d_min_distance(fmod_min_dist);
		internal_channel->set_3d_max_distance(fmod_max_dist);

		// 应用距离滤波器设置（空气吸收效果）
		// custom=true 启用自定义距离滤波器
		internal_channel->set_3d_distance_filter(true, attenuation_filter_db, attenuation_filter_cutoff_hz);

		// 设置衰减模型
		// 注意：FmodChannelControl::set_mode 会覆盖之前的设置
		// 我们需要一次性设置所有模式标志
		unsigned int mode = FmodSystem::FMOD_MODE_3D;
		switch (attenuation_model) {
			case ATTENUATION_INVERSE_DISTANCE:
				mode |= FmodSystem::FMOD_MODE_3D_INVERSEROLLOFF;
				break;
			case ATTENUATION_INVERSE_SQUARE_DISTANCE:
				// FMOD 使用 INVERSETAPEREDROLLOFF 作为平方反比
				mode |= FmodSystem::FMOD_MODE_3D_INVERSETAPEREDROLLOFF;
				break;
			case ATTENUATION_LOGARITHMIC:
				mode |= FmodSystem::FMOD_MODE_3D_LINEARROLLOFF;
				break;
			case ATTENUATION_DISABLED:
				// 不衰减，设置一个极大的最大距离
				internal_channel->set_3d_max_distance(3.402823e+38f);
				mode |= FmodSystem::FMOD_MODE_3D_INVERSEROLLOFF;
				break;
		}
		
		internal_channel->set_mode(static_cast<FmodSystem::FmodMode>(mode));
	}

	void FmodAudioStreamPlayer3D::set_stream(Ref<FmodAudioStream> new_stream) {
		if (internal_channel.is_valid() && internal_channel->channel_is_valid() && internal_channel->is_playing()) {
			internal_channel->stop();
		}
		stream = new_stream;
		playing = false;
	}

	Ref<FmodAudioStream> FmodAudioStreamPlayer3D::get_stream() const {
		return stream;
	}

	void FmodAudioStreamPlayer3D::play(const float from_position) {
		StringName actual_bus = get_bus();
		Ref<FmodAudioBus> audio_bus = FmodServer::get_audio_bus_layout()->get_audio_bus(actual_bus);

		Ref<FmodChannelGroup> channel_group;
		if (audio_bus.is_null()) {
			channel_group = FmodServer::get_master_channel_group();
		} else {
			channel_group = audio_bus->get_bus();
		}

		if (channel_group.is_null()) {
			ERR_PRINT(vformat("Cannot get channel group for bus: %s", actual_bus));
			return;
		}
		internal_channel_group = channel_group;

		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			internal_channel->set_position(int(from_position * 1000));
			internal_channel->set_paused(false);
			playing = true;
			return;
		}

		if (stream.is_valid()) {
			_create_internal_channel(stream);
			if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
				if (from_position > 0.0f) {
					internal_channel->set_position(int(from_position * 1000));
				}
				internal_channel->set_paused(false);
				playing = true;
			}
		}
	}

	void FmodAudioStreamPlayer3D::seek(const float to_position) {
		ERR_FAIL_COND(!internal_channel.is_valid() || !internal_channel->channel_is_valid());
		internal_channel->set_position(int(to_position * 1000));
	}

	void FmodAudioStreamPlayer3D::stop() {
		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			playing = false;
			stream_paused = false;
			internal_channel->stop();
		}
	}

	void FmodAudioStreamPlayer3D::set_playing(const bool p_playing) {
		if (p_playing) {
			play();
		} else {
			stop();
		}
	}

	bool FmodAudioStreamPlayer3D::is_playing() const {
		if (!internal_channel.is_valid() || !internal_channel->channel_is_valid()) return false;
		return playing && internal_channel->is_playing();
	}

	void FmodAudioStreamPlayer3D::set_stream_paused(const bool paused) {
		stream_paused = paused;
		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			internal_channel->set_paused(paused);
		}
	}

	bool FmodAudioStreamPlayer3D::get_stream_paused() const {
		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			return internal_channel->get_paused();
		}
		return stream_paused;
	}

	float FmodAudioStreamPlayer3D::get_playback_position() const {
		if (!internal_channel.is_valid() || !internal_channel->channel_is_valid()) return 0.0f;
		return internal_channel->get_position() / 1000.0f;
	}

	void FmodAudioStreamPlayer3D::set_volume_db(const float new_volume_db) {
		volume_db = new_volume_db;
		if (internal_channel.is_valid() && internal_channel->channel_control_is_valid()) {
			internal_channel->set_volume_db(volume_db);
		}
	}

	float FmodAudioStreamPlayer3D::get_volume_db() const {
		return volume_db;
	}

	void FmodAudioStreamPlayer3D::set_pitch_scale(const float new_pitch) {
		pitch = new_pitch;
		if (internal_channel.is_valid() && internal_channel->channel_control_is_valid()) {
			internal_channel->set_pitch(pitch);
		}
	}

	float FmodAudioStreamPlayer3D::get_pitch_scale() const {
		return pitch;
	}

	void FmodAudioStreamPlayer3D::set_auto_play(const bool enable) {
		auto_play = enable;
	}

	bool FmodAudioStreamPlayer3D::is_autoplay_enabled() const {
		return auto_play;
	}

	void FmodAudioStreamPlayer3D::set_bus(const StringName& p_bus) {
		bus = p_bus;
	}

	StringName FmodAudioStreamPlayer3D::get_bus() const {
		if (bus == StringName()) {
			return "Master";
		}
		return bus;
	}

	void FmodAudioStreamPlayer3D::set_max_distance(const float p_distance) {
		max_distance = Math::max(p_distance, 0.01f);
		if (internal_channel.is_valid()) {
			_apply_attenuation_settings();
		}
	}

	float FmodAudioStreamPlayer3D::get_max_distance() const {
		return max_distance;
	}

	void FmodAudioStreamPlayer3D::set_unit_size(const float p_size) {
		unit_size = Math::max(p_size, 0.001f);
		if (internal_channel.is_valid()) {
			_apply_attenuation_settings();
		}
	}

	float FmodAudioStreamPlayer3D::get_unit_size() const {
		return unit_size;
	}

	void FmodAudioStreamPlayer3D::set_attenuation_model(const AttenuationModel p_model) {
		attenuation_model = p_model;
		if (internal_channel.is_valid()) {
			_apply_attenuation_settings();
		}
	}

	FmodAudioStreamPlayer3D::AttenuationModel FmodAudioStreamPlayer3D::get_attenuation_model() const {
		return attenuation_model;
	}

	void FmodAudioStreamPlayer3D::set_emission_angle_enabled(const bool p_enabled) {
		emission_angle_enabled = p_enabled;
		if (internal_channel.is_valid() && internal_channel->channel_control_is_valid()) {
			_update_3d_attributes();
		}
	}

	bool FmodAudioStreamPlayer3D::is_emission_angle_enabled() const {
		return emission_angle_enabled;
	}

	void FmodAudioStreamPlayer3D::set_emission_angle(const float p_angle) {
		emission_angle = Math::clamp(p_angle, 0.0f, 90.0f);
		if (internal_channel.is_valid() && internal_channel->channel_control_is_valid() && emission_angle_enabled) {
			_update_3d_attributes();
		}
	}

	float FmodAudioStreamPlayer3D::get_emission_angle() const {
		return emission_angle;
	}

	void FmodAudioStreamPlayer3D::set_emission_angle_filter_attenuation_db(const float p_db) {
		emission_angle_filter_attenuation_db = Math::clamp(p_db, -80.0f, 0.0f);
		if (internal_channel.is_valid() && internal_channel->channel_control_is_valid() && emission_angle_enabled) {
			_update_3d_attributes();
		}
	}

	float FmodAudioStreamPlayer3D::get_emission_angle_filter_attenuation_db() const {
		return emission_angle_filter_attenuation_db;
	}

	void FmodAudioStreamPlayer3D::set_attenuation_filter_cutoff_hz(const float p_freq) {
		attenuation_filter_cutoff_hz = Math::clamp(p_freq, 10.0f, 22050.0f);
		if (internal_channel.is_valid() && internal_channel->channel_control_is_valid()) {
			internal_channel->set_3d_distance_filter(true, attenuation_filter_db, attenuation_filter_cutoff_hz);
		}
	}

	float FmodAudioStreamPlayer3D::get_attenuation_filter_cutoff_hz() const {
		return attenuation_filter_cutoff_hz;
	}

	void FmodAudioStreamPlayer3D::set_attenuation_filter_db(const float p_db) {
		attenuation_filter_db = Math::clamp(p_db, -80.0f, 0.0f);
		if (internal_channel.is_valid() && internal_channel->channel_control_is_valid()) {
			internal_channel->set_3d_distance_filter(true, attenuation_filter_db, attenuation_filter_cutoff_hz);
		}
	}

	float FmodAudioStreamPlayer3D::get_attenuation_filter_db() const {
		return attenuation_filter_db;
	}

	void FmodAudioStreamPlayer3D::set_doppler_tracking(const DopplerTracking p_tracking) {
		doppler_tracking = p_tracking;
		set_process(doppler_tracking == DOPPLER_TRACKING_IDLE_STEP);
		set_physics_process(doppler_tracking == DOPPLER_TRACKING_PHYSICS_STEP);
		
		// 立即更新多普勒级别
		if (internal_channel.is_valid() && internal_channel->channel_control_is_valid()) {
			internal_channel->set_3d_doppler_level(
				doppler_tracking == DOPPLER_TRACKING_DISABLED ? 0.0f : 1.0f
			);
		}
	}

	FmodAudioStreamPlayer3D::DopplerTracking FmodAudioStreamPlayer3D::get_doppler_tracking() const {
		return doppler_tracking;
	}

	void FmodAudioStreamPlayer3D::set_area_mask(const uint32_t p_mask) {
		area_mask = p_mask;
	}

	uint32_t FmodAudioStreamPlayer3D::get_area_mask() const {
		return area_mask;
	}
}
