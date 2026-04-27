#include "fmod_channel.h"

namespace godot {
	void FmodChannel::_bind_methods() {
		ClassDB::bind_method(D_METHOD("channel_is_valid"), &FmodChannel::channel_is_valid);
		ClassDB::bind_method(D_METHOD("channel_is_null"), &FmodChannel::channel_is_null);

		ClassDB::bind_method(D_METHOD("set_frequency", "frequency"), &FmodChannel::set_frequency);
		ClassDB::bind_method(D_METHOD("get_frequency"), &FmodChannel::get_frequency);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "frequency"), "set_frequency", "get_frequency");

		ClassDB::bind_method(D_METHOD("set_priority", "priority"), &FmodChannel::set_priority);
		ClassDB::bind_method(D_METHOD("get_priority"), &FmodChannel::get_priority);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "priority"), "set_priority", "get_priority");

		ClassDB::bind_method(D_METHOD("set_position", "position", "timeunit"), &FmodChannel::set_position, DEFVAL(FmodSystem::FMOD_TIME_UNIT_MS));
		ClassDB::bind_method(D_METHOD("get_position", "timeunit"), &FmodChannel::get_position, DEFVAL(FmodSystem::FMOD_TIME_UNIT_MS));

		ClassDB::bind_method(D_METHOD("set_channel_group", "channel_group"), &FmodChannel::set_channel_group);
		ClassDB::bind_method(D_METHOD("get_channel_group"), &FmodChannel::get_channel_group);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "channel_group", PROPERTY_HINT_OBJECT_ID, "FmodChannelGroup"), "set_channel_group", "get_channel_group");

		ClassDB::bind_method(D_METHOD("set_loop_count", "loop_count"), &FmodChannel::set_loop_count);
		ClassDB::bind_method(D_METHOD("get_loop_count"), &FmodChannel::get_loop_count);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "loop_count"), "set_loop_count", "get_loop_count");

		ClassDB::bind_method(D_METHOD("set_loop_points", "start", "end", "timeunit"), &FmodChannel::set_loop_points, DEFVAL(FmodSystem::FMOD_TIME_UNIT_MS));
		ClassDB::bind_method(D_METHOD("get_loop_points"), &FmodChannel::get_loop_points);

		ClassDB::bind_method(D_METHOD("is_virtual"), &FmodChannel::is_virtual);
		ClassDB::bind_method(D_METHOD("get_current_sound"), &FmodChannel::get_current_sound);
		ClassDB::bind_method(D_METHOD("get_index"), &FmodChannel::get_index);

		ADD_SIGNAL(MethodInfo("ended"));
		ADD_SIGNAL(MethodInfo("virtualvoice", PropertyInfo(Variant::BOOL, "is_becoming_virtual")));
		ADD_SIGNAL(MethodInfo("syncpoint", PropertyInfo(Variant::INT, "sync_point_index")));
		ADD_SIGNAL(MethodInfo("occlusion", PropertyInfo(Variant::FLOAT, "direct_occlusion"), PropertyInfo(Variant::FLOAT, "reverb_occlusion")));
	}

	FmodChannel::FmodChannel() {
	
	}

	FmodChannel::~FmodChannel() {
		if (channel) {
			channel->setCallback(nullptr);
			channel->setUserData(nullptr);
			channel = nullptr;
		}
		internal_channel_group.unref();
	}

	void FmodChannel::setup(FMOD::Channel* p_channel) {
		ERR_FAIL_NULL(p_channel);
		channel = p_channel;
		_setup_control(p_channel);
		set_callback();
	}

	bool FmodChannel::channel_is_valid() const {
		return channel != nullptr;
	}

	bool FmodChannel::channel_is_null() const {
		return channel == nullptr;
	}

	void FmodChannel::set_frequency(const double frequency) {
		ERR_FAIL_COND(!channel);
		ERR_FAIL_COND_MSG(frequency <= 0.0, "frequency must be greater than 0.");
		FMOD_ERR_CHECK(channel->setFrequency((float)frequency));
	}

	double FmodChannel::get_frequency() const {
		if (!channel) return 0.0;
		float frequency = 0.0;
		FMOD_ERR_CHECK(channel->getFrequency(&frequency));
		return (double)frequency;
	}

	void FmodChannel::set_priority(const int priority) {
		ERR_FAIL_COND(!channel);
		FMOD_ERR_CHECK(channel->setPriority((int)priority));
	}

	int FmodChannel::get_priority() const {
		if (!channel) return 0;
		int priority = 0;
		FMOD_ERR_CHECK(channel->getPriority(&priority));
		return (int)priority;
	}

	void FmodChannel::set_position(int position, FmodSystem::FmodTimeUnit timeunit) {
		ERR_FAIL_COND(!channel);
		ERR_FAIL_COND_MSG(position < 0, "position must be >= 0.");
		FMOD_ERR_CHECK(channel->setPosition(position, timeunit));
	}

	int FmodChannel::get_position(FmodSystem::FmodTimeUnit timeunit) const {
		if (!channel) return 0.0;
		unsigned int position = 0;
		FMOD_ERR_CHECK(channel->getPosition(&position, timeunit));
		return position;
	}

	void FmodChannel::set_channel_group(Ref<FmodChannelGroup> p_channel_group) {
		ERR_FAIL_COND(!channel);
		ERR_FAIL_COND_MSG(p_channel_group.is_null() || !p_channel_group->channel_group, "ChannelGroup is invalid.");
		internal_channel_group = p_channel_group;
		FMOD_ERR_CHECK(channel->setChannelGroup(internal_channel_group->channel_group));
	}

	Ref<FmodChannelGroup> FmodChannel::get_channel_group() const {
		if (!channel) return nullptr;
		return internal_channel_group;
	}

	void FmodChannel::set_loop_count(const int loop_count) {
		ERR_FAIL_COND(!channel);
		FMOD_ERR_CHECK(channel->setLoopCount(loop_count));
	}

	int FmodChannel::get_loop_count() const {
		if (!channel) return 0;
		int loop_count = 0;
		FMOD_ERR_CHECK(channel->getLoopCount(&loop_count));
		return loop_count;
	}

	void FmodChannel::set_loop_points(const unsigned int start, const unsigned int end, FmodSystem::FmodTimeUnit timeunit) {
		ERR_FAIL_COND(!channel);
		ERR_FAIL_COND_MSG(end < start, "Loop end must be greater than or equal to start.");
		FMOD_TIMEUNIT fmod_timeunit = static_cast<FMOD_TIMEUNIT>((int)timeunit);
		FMOD_ERR_CHECK(channel->setLoopPoints((unsigned int)start, fmod_timeunit, (unsigned int)end, fmod_timeunit));
	}

	Dictionary FmodChannel::get_loop_points(FmodSystem::FmodTimeUnit timeunit) const {
		if (!channel) return Dictionary();
		FMOD_TIMEUNIT fmod_timeunit = static_cast<FMOD_TIMEUNIT>((int)timeunit);
		unsigned int start, end;
		FMOD_ERR_CHECK(channel->getLoopPoints(&start, fmod_timeunit, &end, fmod_timeunit));
		Dictionary result;
		result["start"] = start;
		result["end"] = end;
		return result;
	}

	bool FmodChannel::is_virtual() const {
		if (!channel) return false;
		bool isvirtual = false;
		FMOD_ERR_CHECK(channel->isVirtual(&isvirtual));
		return isvirtual;
	}

	Ref<FmodSound> FmodChannel::get_current_sound() const {
		if (!channel) return nullptr;
		FMOD::Sound* sound_ptr = nullptr;
		FMOD_ERR_CHECK_V(channel->getCurrentSound(&sound_ptr), Ref<FmodSound>());
		ERR_FAIL_NULL_V(sound_ptr, Ref<FmodSound>());

		// 尝试从 userdata 获取已有对象
		void* userdata = nullptr;
		sound_ptr->getUserData(&userdata);
		if (userdata) {
			return Ref<FmodSound>(static_cast<FmodSound*>(userdata));
		}

		Ref<FmodSound> sound;
		sound.instantiate();
		sound->setup(sound_ptr);
		return sound;
	}

	int FmodChannel::get_index() const {
		if (!channel) return -1;
		int index = -1;
		FMOD_ERR_CHECK(channel->getIndex(&index));
		return index;
	}

	void FmodChannel::_on_callback(
		FMOD_CHANNELCONTROL_TYPE controltype,
		FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
		void* commanddata1,
		void* commanddata2) {
		switch (callbacktype) {
		case FMOD_CHANNELCONTROL_CALLBACK_END:
			call_deferred("emit_signal", StringName("ended"));
			break;

		case FMOD_CHANNELCONTROL_CALLBACK_VIRTUALVOICE: {
			ERR_FAIL_NULL(commanddata1);
			int virtual_state = *reinterpret_cast<int*>(commanddata1);
			call_deferred("emit_signal", StringName("virtualvoice"), virtual_state == 1);
			break;
		}

		case FMOD_CHANNELCONTROL_CALLBACK_SYNCPOINT: {
			ERR_FAIL_NULL(commanddata1);
			call_deferred("emit_signal", StringName("syncpoint"), static_cast<int64_t>(reinterpret_cast<uintptr_t>(commanddata1)));
			break;
		}

		case FMOD_CHANNELCONTROL_CALLBACK_OCCLUSION: {
			ERR_FAIL_COND(!commanddata1 || !commanddata2);
			float* direct_ptr = reinterpret_cast<float*>(commanddata1);
			float* reverb_ptr = reinterpret_cast<float*>(commanddata2);

			// 应用遮蔽配置
			float modified_direct = (*direct_ptr) * occlusion_scale + direct_occlusion_bias;
			float modified_reverb = (*reverb_ptr) * occlusion_scale + reverb_occlusion_bias;

			modified_direct = CLAMP(modified_direct, min_direct_occlusion, max_direct_occlusion);
			modified_reverb = CLAMP(modified_reverb, min_reverb_occlusion, max_reverb_occlusion);

			if (smooth_occlusion) {
				current_direct = Math::lerp(current_direct, modified_direct, delta * occlusion_speed);
				current_reverb = Math::lerp(current_reverb, modified_reverb, delta * occlusion_speed);
				*direct_ptr = current_direct;
				*reverb_ptr = current_reverb;
			}
			else {
				*direct_ptr = modified_direct;
				*reverb_ptr = modified_reverb;
			}

			call_deferred("emit_signal", StringName("occlusion"), *direct_ptr, *reverb_ptr);
			break;
		}
		}
	}
}
