#ifndef SERVER_H
#define SERVER_H

#include "fmod_system.h"
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/performance.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
	class FmodAudioBus;
	class FmodAudioBusLayout;
	class FmodAudioEffect;

	class FmodServer : public Object {
		GDCLASS(FmodServer, Object)
		static FmodServer* singleton;

	private:
		void _build_bus_layout();

		double _get_dsp() const;
		double _get_stream() const;
		double _get_geometry() const;
		double _get_update() const;
		double _get_convolution1() const;
		double _get_convolution2() const;
		int64_t _get_sample_bytes_read() const;
		int64_t _get_stream_bytes_read() const;
		int64_t _get_other_bytes_read() const;

		void _connect_update();
		void _update_fmod();

	protected:
		static void _bind_methods();
		void _notification(int p_what);

	public:
		FmodServer();
		~FmodServer();

		static Ref<FmodSystem> main_system;
		static Ref<FmodAudioBusLayout> audio_bus_layout;

		static FmodServer* get_singleton();

		// 获取
		static Ref<FmodSystem> get_main_system();
		static Ref<FmodChannelGroup> get_master_channel_group();
		static Ref<FmodAudioBusLayout> get_audio_bus_layout();
		static void generate_bus_layout();
	};
}

#endif // !SERVER_H
