#include "nodes/fmod_audio_stream_player_2d.h"
#include "mixer/fmod_audio_bus_layout.h"
#include "core/fmod_utils.hpp"
#include <godot_cpp/classes/audio_server.hpp>

namespace godot {
	void FmodAudioStreamPlayer2D::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_stream", "stream"), &FmodAudioStreamPlayer2D::set_stream);
		ClassDB::bind_method(D_METHOD("get_stream"), &FmodAudioStreamPlayer2D::get_stream);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "stream", PROPERTY_HINT_RESOURCE_TYPE, "FmodAudioStream"), "set_stream", "get_stream");

		ClassDB::bind_method(D_METHOD("play", "from_position"), &FmodAudioStreamPlayer2D::play, DEFVAL(0.0));
		ClassDB::bind_method(D_METHOD("seek", "to_position"), &FmodAudioStreamPlayer2D::seek);
		ClassDB::bind_method(D_METHOD("stop"), &FmodAudioStreamPlayer2D::stop);

		ClassDB::bind_method(D_METHOD("set_playing", "playing"), &FmodAudioStreamPlayer2D::set_playing);
		ClassDB::bind_method(D_METHOD("is_playing"), &FmodAudioStreamPlayer2D::is_playing);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "set_playing", "is_playing");

		ClassDB::bind_method(D_METHOD("set_stream_paused", "paused"), &FmodAudioStreamPlayer2D::set_stream_paused);
		ClassDB::bind_method(D_METHOD("get_stream_paused"), &FmodAudioStreamPlayer2D::get_stream_paused);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "stream_paused"), "set_stream_paused", "get_stream_paused");

		ClassDB::bind_method(D_METHOD("get_playback_position"), &FmodAudioStreamPlayer2D::get_playback_position);

		ClassDB::bind_method(D_METHOD("set_volume_db", "volume_db"), &FmodAudioStreamPlayer2D::set_volume_db);
		ClassDB::bind_method(D_METHOD("get_volume_db"), &FmodAudioStreamPlayer2D::get_volume_db);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume_db", PROPERTY_HINT_RANGE, "-80,24,suffix:dB"), "set_volume_db", "get_volume_db");

		ClassDB::bind_method(D_METHOD("set_volume_linear", "volume_linear"), &FmodAudioStreamPlayer2D::set_volume_linear);
		ClassDB::bind_method(D_METHOD("get_volume_linear"), &FmodAudioStreamPlayer2D::get_volume_linear);

		ClassDB::bind_method(D_METHOD("set_pitch_scale", "pitch_scale"), &FmodAudioStreamPlayer2D::set_pitch_scale);
		ClassDB::bind_method(D_METHOD("get_pitch_scale"), &FmodAudioStreamPlayer2D::get_pitch_scale);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch_scale", PROPERTY_HINT_RANGE, "0.01,4,0.01,or_greater"), "set_pitch_scale", "get_pitch_scale");

		ClassDB::bind_method(D_METHOD("set_autoplay", "enable"), &FmodAudioStreamPlayer2D::set_autoplay);
		ClassDB::bind_method(D_METHOD("is_autoplay_enabled"), &FmodAudioStreamPlayer2D::is_autoplay_enabled);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autoplay"), "set_autoplay", "is_autoplay_enabled");

		ClassDB::bind_method(D_METHOD("set_max_distance", "pixels"), &FmodAudioStreamPlayer2D::set_max_distance);
		ClassDB::bind_method(D_METHOD("get_max_distance"), &FmodAudioStreamPlayer2D::get_max_distance);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_distance", PROPERTY_HINT_RANGE, "1,4096,1,or_greater,exp,suffix:px"), "set_max_distance", "get_max_distance");

		ClassDB::bind_method(D_METHOD("set_attenuation", "curve"), &FmodAudioStreamPlayer2D::set_attenuation);
		ClassDB::bind_method(D_METHOD("get_attenuation"), &FmodAudioStreamPlayer2D::get_attenuation);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attenuation", PROPERTY_HINT_EXP_EASING, "attenuation"), "set_attenuation", "get_attenuation");

		ClassDB::bind_method(D_METHOD("set_panning_strength", "panning_strength"), &FmodAudioStreamPlayer2D::set_panning_strength);
		ClassDB::bind_method(D_METHOD("get_panning_strength"), &FmodAudioStreamPlayer2D::get_panning_strength);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "panning_strength", PROPERTY_HINT_RANGE, "0,3,0.01,or_greater"), "set_panning_strength", "get_panning_strength");

		ClassDB::bind_method(D_METHOD("set_bus", "bus"), &FmodAudioStreamPlayer2D::set_bus);
		ClassDB::bind_method(D_METHOD("get_bus"), &FmodAudioStreamPlayer2D::get_bus);
		ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "bus", PROPERTY_HINT_ENUM), "set_bus", "get_bus");

		ClassDB::bind_method(D_METHOD("set_area_mask", "mask"), &FmodAudioStreamPlayer2D::set_area_mask);
		ClassDB::bind_method(D_METHOD("get_area_mask"), &FmodAudioStreamPlayer2D::get_area_mask);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "area_mask", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_area_mask", "get_area_mask");

		ClassDB::bind_method(D_METHOD("set_max_polyphony", "max_polyphony"), &FmodAudioStreamPlayer2D::set_max_polyphony);
		ClassDB::bind_method(D_METHOD("get_max_polyphony"), &FmodAudioStreamPlayer2D::get_max_polyphony);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "max_polyphony"), "set_max_polyphony", "get_max_polyphony");

		ClassDB::bind_method(D_METHOD("has_stream_playback"), &FmodAudioStreamPlayer2D::has_stream_playback);
		ClassDB::bind_method(D_METHOD("get_stream_playback"), &FmodAudioStreamPlayer2D::get_stream_playback);

		ADD_SIGNAL(MethodInfo("finished"));
	}

	FmodAudioStreamPlayer2D::FmodAudioStreamPlayer2D() {
		internal_channel.instantiate();
		if (FmodServer::get_singleton()) {
			Ref<FmodSystem> system = FmodServer::get_main_system();
			if (system.is_valid()) {
				internal_channel_group = system->get_master_channel_group();
			}
		}
		// 启用处理以便更新 2D 属性
		set_process(true);
		set_physics_process(true);
	}

	FmodAudioStreamPlayer2D::~FmodAudioStreamPlayer2D() {
		if (internal_channel.is_valid()) {
			internal_channel->stop();
		}
	}

	void FmodAudioStreamPlayer2D::_notification(int p_what) {
		switch (p_what) {
		case NOTIFICATION_READY: {
			if (auto_play && stream.is_valid() && !Engine::get_singleton()->is_editor_hint()) {
				call_deferred("play");
			}
			last_position = get_global_position();
		} break;

		case NOTIFICATION_PHYSICS_PROCESS: {
			// 更新位置和声像
			if (internal_channel.is_valid() && internal_channel->is_playing()) {
				_update_panning();
			}
		} break;
		}
	}

	void FmodAudioStreamPlayer2D::_validate_property(PropertyInfo& p_property) const {
		if (p_property.name == StringName("bus")) {
			// 动态构建枚举字符串
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

	void FmodAudioStreamPlayer2D::_update_panning() {
		if (!internal_channel.is_valid() || !internal_channel->is_playing() || !is_inside_tree()) {
			return;
		}

		Vector2 global_pos = get_global_position();
		last_position = global_pos;

		// 从 FmodServer 获取摄像机位置和旋转
		Vector2 listener_pos = FmodServer::camera_2d_pos;
		float listener_rotation = FmodServer::camera_2d_rot;

		Viewport* viewport = get_viewport();
		Camera2D* camera = nullptr;
		if (viewport && listener_pos == Vector2() && listener_rotation == 0.0f) {
			camera = viewport->get_camera_2d();
			if (camera) {
				listener_pos = camera->get_global_position();
				listener_rotation = camera->get_global_rotation();
			}
		}

		// 计算距离
		float dist = global_pos.distance_to(listener_pos);
		if (dist > max_distance) {
			internal_channel->set_volume_db(-80.0f);
			return;
		}

		// 计算距离衰减和相对位置
		float attenuation_mult = Math::pow(1.0f - dist / max_distance, attenuation);
		Vector2 relative_pos = (global_pos - listener_pos).rotated(-listener_rotation);

		// 计算声像
		float pan = 0.0f;
		if (viewport) {
			Size2 screen_size = viewport->get_visible_rect().size;
			pan = relative_pos.x / screen_size.x;
			pan = CLAMP(pan, -1.0f, 1.0f);
			pan *= panning_strength;
			pan = CLAMP(pan + 0.5f, 0.0f, 1.0f);
		}

		// 应用音量
		float total_volume_db = volume_db;
		if (attenuation_mult > 0.0f) {
			total_volume_db += FmodUtils::linear_to_db(attenuation_mult);
		}
		else {
			total_volume_db = -80.0f;
		}
		internal_channel->set_volume_db(total_volume_db);

		// 应用声像（转换到 -1 到 1 范围）
		internal_channel->set_pan(pan * 2.0f - 1.0f);
	}

	void FmodAudioStreamPlayer2D::_create_internal_channel(Ref<FmodAudioStream> p_stream) {
		Ref<FmodSystem> system = FmodServer::get_main_system();
		if (system.is_null()) {
			ERR_PRINT("FMOD system not available");
			return;
		}

		// 确保声音使用 2D 模式
		p_stream->add_mode_flag(FmodAudioStream::MODE_2D);
		// 使已缓存的声音失效，让它用新标志重新创建（保留数据）
		p_stream->invalidate_sound();

		Ref<FmodSound> sound = p_stream->get_sound();
		if (sound.is_null()) {
			ERR_PRINT("Failed to get sound from stream");
			return;
		}

		internal_channel = system->play_sound(sound, internal_channel_group, true);
		if (internal_channel.is_null()) {
			ERR_PRINT("Failed to create FMOD channel");
			return;
		}

		internal_channel->set_volume_db(volume_db);
		internal_channel->set_pitch(pitch);

		// 设置为 2D 模式，禁用 FMOD 的 3D 处理
		// 这样距离衰减完全由我们手动控制
		internal_channel->set_mode(FmodSystem::FMOD_MODE_2D);

		// 连接结束信号
		internal_channel->connect("ended", callable_mp(this, &FmodAudioStreamPlayer2D::_on_internal_channel_ended), CONNECT_DEFERRED);
	}

	void FmodAudioStreamPlayer2D::_on_internal_channel_ended() {
		if (internal_channel.is_valid()) {
			internal_channel->disconnect("ended", callable_mp(this, &FmodAudioStreamPlayer2D::_on_internal_channel_ended));
		}
		internal_channel.unref();

		// stop 会提前设置 playing 为 false，所以判断是否还在播放就行
		if (playing) {
			playing = false;
			emit_signal("finished");
		}
	}

	void FmodAudioStreamPlayer2D::set_stream(Ref<FmodAudioStream> new_stream) {
		if (internal_channel.is_valid() && internal_channel->is_playing()) {
			internal_channel->stop();
		}
		stream = new_stream;
		playing = false;
	}

	Ref<FmodAudioStream> FmodAudioStreamPlayer2D::get_stream() const {
		return stream;
	}

	void FmodAudioStreamPlayer2D::play(const float from_position) {
		StringName actual_bus = get_bus();
		Ref<FmodAudioBus> audio_bus = FmodServer::get_audio_bus_layout()->get_audio_bus(actual_bus);

		Ref<FmodChannelGroup> channel_group;
		if (audio_bus.is_null()) {
			channel_group = FmodServer::get_master_channel_group();
		}
		else {
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
			if (internal_channel.is_valid()) {
				if (from_position > 0.0f) {
					internal_channel->set_position(int(from_position * 1000));
				}
				internal_channel->set_paused(false);
				playing = true;
			}
		}
	}

	void FmodAudioStreamPlayer2D::seek(const float to_position) {
		if (!internal_channel.is_valid()) {
			return;
		}
		internal_channel->set_position(int(to_position * 1000));
	}

	void FmodAudioStreamPlayer2D::stop() {
		if (internal_channel.is_valid()) {
			playing = false;
			stream_paused = false;
			internal_channel->stop();
		}
	}

	void FmodAudioStreamPlayer2D::set_playing(const bool p_playing) {
		if (p_playing) {
			play();
		}
		else {
			stop();
		}
	}

	bool FmodAudioStreamPlayer2D::is_playing() const {
		if (internal_channel.is_null()) return false;
		return playing && internal_channel->is_playing();
	}

	void FmodAudioStreamPlayer2D::set_stream_paused(const bool paused) {
		stream_paused = paused;
		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			internal_channel->set_paused(paused);
		}
	}

	bool FmodAudioStreamPlayer2D::get_stream_paused() const {
		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			return internal_channel->get_paused();
		}
		return stream_paused;
	}

	float FmodAudioStreamPlayer2D::get_playback_position() const {
		if (internal_channel.is_null()) return 0.0f;
		return internal_channel->get_position() / 1000.0f;
	}

	void FmodAudioStreamPlayer2D::set_volume_db(const float new_volume_db) {
		volume_db = new_volume_db;
		if (internal_channel.is_valid()) {
			internal_channel->set_volume_db(volume_db);
		}
	}

	float FmodAudioStreamPlayer2D::get_volume_db() const {
		return volume_db;
	}

	void FmodAudioStreamPlayer2D::set_volume_linear(const float p_volume) {
		set_volume_db(FmodUtils::linear_to_db(p_volume));
	}

	float FmodAudioStreamPlayer2D::get_volume_linear() const {
		return FmodUtils::db_to_linear(get_volume_db());
	}

	void FmodAudioStreamPlayer2D::set_pitch_scale(const float new_pitch) {
		pitch = new_pitch;
		if (internal_channel.is_valid()) {
			internal_channel->set_pitch(pitch);
		}
	}

	float FmodAudioStreamPlayer2D::get_pitch_scale() const {
		return pitch;
	}

	void FmodAudioStreamPlayer2D::set_autoplay(const bool enable) {
		auto_play = enable;
	}

	bool FmodAudioStreamPlayer2D::is_autoplay_enabled() const {
		return auto_play;
	}

	void FmodAudioStreamPlayer2D::set_max_distance(float p_pixels) {
		if (p_pixels <= 0.0f) {
			p_pixels = 1.0f;
		}
		max_distance = p_pixels;
	}

	float FmodAudioStreamPlayer2D::get_max_distance() const {
		return max_distance;
	}

	void FmodAudioStreamPlayer2D::set_attenuation(float p_curve) {
		attenuation = p_curve;
	}

	float FmodAudioStreamPlayer2D::get_attenuation() const {
		return attenuation;
	}

	void FmodAudioStreamPlayer2D::set_panning_strength(float p_panning_strength) {
		if (p_panning_strength < 0.0f) {
			p_panning_strength = 0.0f;
		}
		panning_strength = p_panning_strength;
	}

	float FmodAudioStreamPlayer2D::get_panning_strength() const {
		return panning_strength;
	}

	void FmodAudioStreamPlayer2D::set_bus(const StringName& p_bus) {
		bus = p_bus;
	}

	StringName FmodAudioStreamPlayer2D::get_bus() const {
		if (bus == StringName()) {
			return "Master";
		}
		return bus;
	}

	void FmodAudioStreamPlayer2D::set_area_mask(uint32_t p_mask) {
		area_mask = p_mask;
	}

	uint32_t FmodAudioStreamPlayer2D::get_area_mask() const {
		return area_mask;
	}

	void FmodAudioStreamPlayer2D::set_max_polyphony(int p_max_polyphony) {
		if (p_max_polyphony < 1) {
			p_max_polyphony = 1;
		}
		max_polyphony = p_max_polyphony;
	}

	int FmodAudioStreamPlayer2D::get_max_polyphony() const {
		return max_polyphony;
	}

	bool FmodAudioStreamPlayer2D::has_stream_playback() const {
		return internal_channel.is_valid();
	}

	Ref<FmodChannel> FmodAudioStreamPlayer2D::get_stream_playback() const {
		return internal_channel;
	}
}
