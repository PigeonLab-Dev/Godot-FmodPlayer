#ifndef FMOD_AUDIO_STREAM_H
#define FMOD_AUDIO_STREAM_H

#include "audio/fmod_sound.h"
#include "core/fmod_server.h"
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {
	class FmodAudioStream : public Resource {
		GDCLASS(FmodAudioStream, Resource)

	public:
		// FMOD 创建模式标志
		enum CreateMode {
			MODE_DEFAULT = 0,
			MODE_STREAM = 1 << 0,       // 流式加载（从磁盘流式播放）
			MODE_SAMPLE = 1 << 1,       // 样本模式（加载到内存）
			MODE_LOOP = 1 << 2,         // 循环播放
			MODE_LOOP_BIDI = 1 << 3,    // 双向循环
			MODE_2D = 1 << 4,           // 强制 2D 模式（禁用 3D 处理）
			MODE_3D = 1 << 5,           // 强制 3D 模式
		};

	protected:
		static void _bind_methods();

		// 音频数据（由导入器填充或运行时加载）
		PackedByteArray audio_data;
		bool data_loaded = false;
		Ref<FmodSound> sound;
		unsigned int create_mode_flags = MODE_STREAM;

	public:
		FmodAudioStream();
		virtual ~FmodAudioStream();

		virtual Ref<FmodSound> _create_sound();
		void set_audio_data(const PackedByteArray& p_data);
		PackedByteArray get_audio_data() const;

		// 获取/设置创建模式标志
		void set_mode_flags(int p_flags);
		int get_mode_flags() const;
		void add_mode_flag(CreateMode p_flag);
		void remove_mode_flag(CreateMode p_flag);
		bool has_mode_flag(CreateMode p_flag) const;

		// 获取 Sound（延迟创建）
		Ref<FmodSound> get_sound();
		bool preload();
		bool is_preloaded() const;

		// 音频信息
		double get_length() const;
		bool is_data_loaded() const;

		// 运行时加载外部文件
		static Ref<FmodAudioStream> load_from_file(const String& p_path, int p_flags = MODE_STREAM);

		// 清理资源
		void clear();

		// 仅使 sound 缓存失效（保留数据），下次 get_sound() 会重新创建
		void invalidate_sound();
	};
}

VARIANT_ENUM_CAST(FmodAudioStream::CreateMode);

#endif // !FMOD_AUDIO_STREAM_H
