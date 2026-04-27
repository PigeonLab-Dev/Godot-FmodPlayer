#include "fmod_channel_group.h"

namespace godot {
	void FmodChannelGroup::_bind_methods() {
		ClassDB::bind_method(D_METHOD("channel_group_is_valid"), &FmodChannelGroup::channel_group_is_valid);
		ClassDB::bind_method(D_METHOD("channel_group_is_null"), &FmodChannelGroup::channel_group_is_null);

		ClassDB::bind_method(D_METHOD("get_num_channels"), &FmodChannelGroup::get_num_channels);
		ClassDB::bind_method(D_METHOD("get_channel", "index"), &FmodChannelGroup::get_channel);

		ClassDB::bind_method(D_METHOD("add_group", "child_group", "propagatedspclock"), &FmodChannelGroup::add_group, DEFVAL(true));
		ClassDB::bind_method(D_METHOD("get_num_groups"), &FmodChannelGroup::get_num_groups);
		ClassDB::bind_method(D_METHOD("get_group", "index"), &FmodChannelGroup::get_group);
		ClassDB::bind_method(D_METHOD("get_parent_group"), &FmodChannelGroup::get_parent_group);

		ClassDB::bind_method(D_METHOD("get_name"), &FmodChannelGroup::get_name);
		ClassDB::bind_method(D_METHOD("release"), &FmodChannelGroup::release);
	}

	FmodChannelGroup::FmodChannelGroup() {

	}
	
	FmodChannelGroup::~FmodChannelGroup() {
		if (channel_group) {
			channel_group->setCallback(nullptr);
			channel_group->setUserData(nullptr);
			channel_group = nullptr;
			channel_control = nullptr;
		}
	}

	void FmodChannelGroup::setup(FMOD::ChannelGroup* p_channel_group) {
		ERR_FAIL_NULL(p_channel_group);
		_setup_control(p_channel_group);  // 调用基类方法设置 channel_control
		channel_group = p_channel_group;  // 存储派生类特有的指针
	}

	bool FmodChannelGroup::channel_group_is_valid() const {
		return channel_group != nullptr;
	}

	bool FmodChannelGroup::channel_group_is_null() const {
		return channel_group == nullptr;
	}

	int FmodChannelGroup::get_num_channels() const {
		ERR_FAIL_COND_V(!channel_group, -1);
		int numchannels = -1;
		FMOD_ERR_CHECK(channel_group->getNumChannels(&numchannels));
		return numchannels;
	}

	Ref<FmodChannel> FmodChannelGroup::get_channel(const int index) const {
		ERR_FAIL_COND_V(!channel_group, Ref<FmodChannel>());
		ERR_FAIL_COND_V_MSG(index < 0, Ref<FmodChannel>(), "Channel index must be >= 0.");
		FMOD::Channel* p_channel = nullptr;
		FMOD_ERR_CHECK_V(channel_group->getChannel(index, &p_channel), Ref<FmodChannel>());
		ERR_FAIL_NULL_V(p_channel, Ref<FmodChannel>());

		// 尝试从 userdata 获取已有对象
		void* userdata = nullptr;
		p_channel->getUserData(&userdata);
		if (userdata) {
			return Ref<FmodChannel>(static_cast<FmodChannel*>(userdata));
		}

		Ref<FmodChannel> channel;
		channel.instantiate();
		channel->setup(p_channel);
		return channel;
	}

	void FmodChannelGroup::add_group(Ref<FmodChannelGroup> child_group, bool propagatedspclock) {
		ERR_FAIL_COND(!channel_group);
		ERR_FAIL_COND_MSG(child_group.is_null() || !child_group->channel_group,
			"Child group is invalid");

		FMOD_RESULT result = channel_group->addGroup(child_group->channel_group, propagatedspclock);
		if (result != FMOD_OK) {
			ERR_PRINT(vformat("Failed to add group: %s", FMOD_ErrorString(result)));
		}
	}

	int FmodChannelGroup::get_num_groups() const {
		ERR_FAIL_COND_V(!channel_group, 0);
		int numgroups = 0;
		FMOD_ERR_CHECK_V(channel_group->getNumGroups(&numgroups), 0);
		return numgroups;
	}

	Ref<FmodChannelGroup> FmodChannelGroup::get_group(const int index) const {
		ERR_FAIL_COND_V(!channel_group, Ref<FmodChannelGroup>());
		ERR_FAIL_COND_V_MSG(index < 0, Ref<FmodChannelGroup>(), "Group index must be >= 0.");
		FMOD::ChannelGroup* channel_group_ptr = nullptr;
		FMOD_ERR_CHECK_V(channel_group->getGroup(index, &channel_group_ptr), Ref<FmodChannelGroup>());
		ERR_FAIL_NULL_V(channel_group_ptr, Ref<FmodChannelGroup>());
		Ref<FmodChannelGroup> channel_group;
		channel_group.instantiate();
		channel_group->setup(channel_group_ptr);
		return channel_group;
	}

	Ref<FmodChannelGroup> FmodChannelGroup::get_parent_group() const {
		ERR_FAIL_COND_V(!channel_group, Ref<FmodChannelGroup>());
		FMOD::ChannelGroup* channel_group_ptr = nullptr;
		FMOD_ERR_CHECK_V(channel_group->getParentGroup(&channel_group_ptr), Ref<FmodChannelGroup>());
		if (!channel_group_ptr) return Ref<FmodChannelGroup>();
		Ref<FmodChannelGroup> channel_group;
		channel_group.instantiate();
		channel_group->setup(channel_group_ptr);
		return channel_group;
	}

	String FmodChannelGroup::get_name() const {
		ERR_FAIL_COND_V(!channel_group, String());
		char name[512] = { 0 };
		FMOD_ERR_CHECK_V(channel_group->getName(name, 512), String());
		return String::utf8(name);
	}

	void FmodChannelGroup::release() {
		ERR_FAIL_COND_MSG(!channel_group, "ChannelGroup is null");
		channel_group->setCallback(nullptr);
		channel_group->setUserData(nullptr);
		FMOD_ERR_CHECK(channel_group->release());
		channel_group = nullptr;
		channel_control = nullptr;
	}
}
