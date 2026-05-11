#ifndef FMOD_CHANNEL_GROUP_H
#define FMOD_CHANNEL_GROUP_H

#include "core/fmod_system.h"
#include "playback/fmod_channel.h"
#include "playback/fmod_channel_control.h"
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {
	class FmodChannelGroup : public FmodChannelControl {
		GDCLASS(FmodChannelGroup, FmodChannelControl)

	private:

	protected:
		static void _bind_methods();

	public:
		FmodChannelGroup();
		~FmodChannelGroup();

		FMOD::ChannelGroup* channel_group = nullptr;

		void setup(FMOD::ChannelGroup* p_channel_group);

		bool channel_group_is_valid() const;								// 检查 ChannelGroup 是否有效
		bool channel_group_is_null() const;									// 检查 ChannelGroup 是否无效

		// Channel 管理
		int get_num_channels() const;										// 获取汇入该组的 Channel 数量
		Ref<FmodChannel> get_channel(const int index) const;				// 获取指定索引处的 Channel

		// ChannelGroup 管理
		void add_group(
			Ref<FmodChannelGroup> channel_group,
			bool propagatedspclock = true
		);																	// 向该组添加一个 ChannelGroup 作为输入
		int get_num_groups() const;											// 获取汇入该组的 ChannelGroup 数量
		Ref<FmodChannelGroup> get_group(const int index) const;				// 在组输入列表中指定的索引处获取 ChannelGroup
		Ref<FmodChannelGroup> get_parent_group() const;						// 获取该对象输出的 ChannelGroup

		// 概述
		String get_name() const;											// 获取组创建时的名称
		void release();														// 释放 ChannelGroup
	};
}

#endif // !FMOD_CHANNEL_GROUP_H