#ifndef FMOD_AUDIO_BUS_LAYOUT_H
#define FMOD_AUDIO_BUS_LAYOUT_H

#include "mixer/fmod_audio_bus.h"
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/templates/hash_map.hpp>

namespace godot {
	class FmodAudioEffect;

	class FmodAudioBusLayout : public Resource {
		GDCLASS(FmodAudioBusLayout, Resource)

	private:
		HashMap<String, Ref<FmodAudioBus>> audio_buses_map;
		String audio_server_layout_signature;

		void _clear_buses_except_master();
		Ref<FmodAudioBus> _ensure_master_bus();
		void _sync_bus_effects(Ref<FmodAudioBus> bus, int audio_server_bus_index);
		void _update_solo_mute();
		String _build_audio_server_layout_signature() const;

	protected:
		static void _bind_methods();

	public:
		// 总线管理
		void create_audio_bus(const String& name, Ref<FmodAudioBus> parent = Ref<FmodAudioBus>());
		Ref<FmodAudioBus> get_audio_bus(const String& name) const;
		bool has_audio_bus(const String& name) const;
		void remove_audio_bus(const String& name);

		void set_bus_volume(const String& name, const float volume_db);
		float get_bus_volume(const String& name) const;

		void set_bus_solo(const String& name, const bool solo);
		bool bus_is_solo(const String& name) const;

		void set_bus_mute(const String& name, const bool mute);
		bool bus_is_mute(const String& name) const;

		void set_bus_bypass(const String& name, const bool bypass);
		bool bus_is_bypass(const String& name) const;

		// 效果器管理
		void add_bus_effect(const String& bus_name, Ref<FmodAudioEffect> effect, const int index = 0);
		void remove_bus_effect(const String& bus_name, const int index);
		int get_bus_effect_count(const String& bus_name) const;
		Ref<FmodAudioEffect> get_bus_effect(const String& bus_name, const int index) const;

		// 与 Godot AudioServer 同步
		void sync_from_audio_server();
		bool sync_from_audio_server_if_changed();
		void sync_bus_state(const String& bus_name, int audio_server_bus_index) const;
		void clear();
	};
}

#endif // !FMOD_AUDIO_BUS_LAYOUT_H

