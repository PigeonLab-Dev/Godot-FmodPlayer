#ifndef FMOD_REVERB_ZONE_3D_H
#define FMOD_REVERB_ZONE_3D_H

#include "spatial/fmod_reverb_3d.h"
#include <godot_cpp/classes/node3d.hpp>

namespace godot {
	class FmodReverbZone3D : public Node3D {
		GDCLASS(FmodReverbZone3D, Node3D)

	public:
		enum Preset {
			PRESET_CUSTOM = 0,
			PRESET_OFF,
			PRESET_GENERIC,
			PRESET_PADDED_CELL,
			PRESET_ROOM,
			PRESET_BATHROOM,
			PRESET_LIVING_ROOM,
			PRESET_STONE_ROOM,
			PRESET_AUDITORIUM,
			PRESET_CONCERT_HALL,
			PRESET_CAVE,
			PRESET_ARENA,
			PRESET_HANGAR,
			PRESET_HALLWAY,
			PRESET_STONE_CORRIDOR,
			PRESET_ALLEY,
			PRESET_FOREST,
			PRESET_CITY,
			PRESET_MOUNTAINS,
			PRESET_UNDERWATER,
		};

	private:
		Ref<FmodReverb3D> reverb;
		Preset preset = PRESET_GENERIC;
		bool active = true;
		bool sync_transform = true;
		float min_distance = 5.0f;
		float max_distance = 20.0f;

		FMOD_REVERB_PROPERTIES properties = FMOD_PRESET_GENERIC;

		static FMOD_REVERB_PROPERTIES _preset_to_properties(Preset p_preset);
		void _ensure_reverb();
		void _release_reverb();
		void _sync_reverb();
		void _set_custom();
		Dictionary _properties_to_dictionary() const;

	protected:
		static void _bind_methods();
		void _notification(int p_what);

	public:
		FmodReverbZone3D();
		~FmodReverbZone3D();

		void set_active(bool p_active);
		bool get_active() const;

		void set_sync_transform(bool p_sync);
		bool get_sync_transform() const;

		void set_min_distance(float p_distance);
		float get_min_distance() const;

		void set_max_distance(float p_distance);
		float get_max_distance() const;

		void set_preset(Preset p_preset);
		Preset get_preset() const;

		void set_decay_time(float p_value);
		float get_decay_time() const;
		void set_early_delay(float p_value);
		float get_early_delay() const;
		void set_late_delay(float p_value);
		float get_late_delay() const;
		void set_hf_reference(float p_value);
		float get_hf_reference() const;
		void set_hf_decay_ratio(float p_value);
		float get_hf_decay_ratio() const;
		void set_diffusion(float p_value);
		float get_diffusion() const;
		void set_density(float p_value);
		float get_density() const;
		void set_low_shelf_frequency(float p_value);
		float get_low_shelf_frequency() const;
		void set_low_shelf_gain(float p_value);
		float get_low_shelf_gain() const;
		void set_high_cut(float p_value);
		float get_high_cut() const;
		void set_early_late_mix(float p_value);
		float get_early_late_mix() const;
		void set_wet_level(float p_value);
		float get_wet_level() const;

		Ref<FmodReverb3D> get_reverb() const;
	};
}

VARIANT_ENUM_CAST(FmodReverbZone3D::Preset);

#endif // FMOD_REVERB_ZONE_3D_H
