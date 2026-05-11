#include "nodes/fmod_reverb_zone_3d.h"

#include "core/fmod_server.h"
#include "core/fmod_system.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {
	void FmodReverbZone3D::_bind_methods() {
		BIND_ENUM_CONSTANT(PRESET_CUSTOM);
		BIND_ENUM_CONSTANT(PRESET_OFF);
		BIND_ENUM_CONSTANT(PRESET_GENERIC);
		BIND_ENUM_CONSTANT(PRESET_PADDED_CELL);
		BIND_ENUM_CONSTANT(PRESET_ROOM);
		BIND_ENUM_CONSTANT(PRESET_BATHROOM);
		BIND_ENUM_CONSTANT(PRESET_LIVING_ROOM);
		BIND_ENUM_CONSTANT(PRESET_STONE_ROOM);
		BIND_ENUM_CONSTANT(PRESET_AUDITORIUM);
		BIND_ENUM_CONSTANT(PRESET_CONCERT_HALL);
		BIND_ENUM_CONSTANT(PRESET_CAVE);
		BIND_ENUM_CONSTANT(PRESET_ARENA);
		BIND_ENUM_CONSTANT(PRESET_HANGAR);
		BIND_ENUM_CONSTANT(PRESET_HALLWAY);
		BIND_ENUM_CONSTANT(PRESET_STONE_CORRIDOR);
		BIND_ENUM_CONSTANT(PRESET_ALLEY);
		BIND_ENUM_CONSTANT(PRESET_FOREST);
		BIND_ENUM_CONSTANT(PRESET_CITY);
		BIND_ENUM_CONSTANT(PRESET_MOUNTAINS);
		BIND_ENUM_CONSTANT(PRESET_UNDERWATER);

		ClassDB::bind_method(D_METHOD("set_active", "active"), &FmodReverbZone3D::set_active);
		ClassDB::bind_method(D_METHOD("get_active"), &FmodReverbZone3D::get_active);

		ClassDB::bind_method(D_METHOD("set_sync_transform", "sync"), &FmodReverbZone3D::set_sync_transform);
		ClassDB::bind_method(D_METHOD("get_sync_transform"), &FmodReverbZone3D::get_sync_transform);

		ClassDB::bind_method(D_METHOD("set_min_distance", "distance"), &FmodReverbZone3D::set_min_distance);
		ClassDB::bind_method(D_METHOD("get_min_distance"), &FmodReverbZone3D::get_min_distance);

		ClassDB::bind_method(D_METHOD("set_max_distance", "distance"), &FmodReverbZone3D::set_max_distance);
		ClassDB::bind_method(D_METHOD("get_max_distance"), &FmodReverbZone3D::get_max_distance);

		ClassDB::bind_method(D_METHOD("set_preset", "preset"), &FmodReverbZone3D::set_preset);
		ClassDB::bind_method(D_METHOD("get_preset"), &FmodReverbZone3D::get_preset);

		ClassDB::bind_method(D_METHOD("set_decay_time", "value"), &FmodReverbZone3D::set_decay_time);
		ClassDB::bind_method(D_METHOD("get_decay_time"), &FmodReverbZone3D::get_decay_time);
		
		ClassDB::bind_method(D_METHOD("set_early_delay", "value"), &FmodReverbZone3D::set_early_delay);
		ClassDB::bind_method(D_METHOD("get_early_delay"), &FmodReverbZone3D::get_early_delay);
		
		ClassDB::bind_method(D_METHOD("set_late_delay", "value"), &FmodReverbZone3D::set_late_delay);
		ClassDB::bind_method(D_METHOD("get_late_delay"), &FmodReverbZone3D::get_late_delay);
		
		ClassDB::bind_method(D_METHOD("set_hf_reference", "value"), &FmodReverbZone3D::set_hf_reference);
		ClassDB::bind_method(D_METHOD("get_hf_reference"), &FmodReverbZone3D::get_hf_reference);
		
		ClassDB::bind_method(D_METHOD("set_hf_decay_ratio", "value"), &FmodReverbZone3D::set_hf_decay_ratio);
		ClassDB::bind_method(D_METHOD("get_hf_decay_ratio"), &FmodReverbZone3D::get_hf_decay_ratio);
		
		ClassDB::bind_method(D_METHOD("set_diffusion", "value"), &FmodReverbZone3D::set_diffusion);
		ClassDB::bind_method(D_METHOD("get_diffusion"), &FmodReverbZone3D::get_diffusion);
		
		ClassDB::bind_method(D_METHOD("set_density", "value"), &FmodReverbZone3D::set_density);
		ClassDB::bind_method(D_METHOD("get_density"), &FmodReverbZone3D::get_density);
		
		ClassDB::bind_method(D_METHOD("set_low_shelf_frequency", "value"), &FmodReverbZone3D::set_low_shelf_frequency);
		ClassDB::bind_method(D_METHOD("get_low_shelf_frequency"), &FmodReverbZone3D::get_low_shelf_frequency);
		
		ClassDB::bind_method(D_METHOD("set_low_shelf_gain", "value"), &FmodReverbZone3D::set_low_shelf_gain);
		ClassDB::bind_method(D_METHOD("get_low_shelf_gain"), &FmodReverbZone3D::get_low_shelf_gain);
		
		ClassDB::bind_method(D_METHOD("set_high_cut", "value"), &FmodReverbZone3D::set_high_cut);
		ClassDB::bind_method(D_METHOD("get_high_cut"), &FmodReverbZone3D::get_high_cut);
		
		ClassDB::bind_method(D_METHOD("set_early_late_mix", "value"), &FmodReverbZone3D::set_early_late_mix);
		ClassDB::bind_method(D_METHOD("get_early_late_mix"), &FmodReverbZone3D::get_early_late_mix);
		
		ClassDB::bind_method(D_METHOD("set_wet_level", "value"), &FmodReverbZone3D::set_wet_level);
		ClassDB::bind_method(D_METHOD("get_wet_level"), &FmodReverbZone3D::get_wet_level);
		
		ClassDB::bind_method(D_METHOD("get_reverb"), &FmodReverbZone3D::get_reverb);

		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sync_transform"), "set_sync_transform", "get_sync_transform");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_distance", PROPERTY_HINT_RANGE, "0,1000,0.01,or_greater,suffix:m"), "set_min_distance", "get_min_distance");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_distance", PROPERTY_HINT_RANGE, "0.001,1000,0.01,or_greater,suffix:m"), "set_max_distance", "get_max_distance");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "preset", PROPERTY_HINT_ENUM, "Custom,Off,Generic,Padded Cell,Room,Bathroom,Living Room,Stone Room,Auditorium,Concert Hall,Cave,Arena,Hangar,Hallway,Stone Corridor,Alley,Forest,City,Mountains,Underwater"), "set_preset", "get_preset");
		ADD_GROUP("Reverb Properties", "");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "decay_time", PROPERTY_HINT_RANGE, "100,20000,0.1,suffix:ms"), "set_decay_time", "get_decay_time");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "early_delay", PROPERTY_HINT_RANGE, "0,300,0.1,suffix:ms"), "set_early_delay", "get_early_delay");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "late_delay", PROPERTY_HINT_RANGE, "0,100,0.1,suffix:ms"), "set_late_delay", "get_late_delay");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "hf_reference", PROPERTY_HINT_RANGE, "20,20000,1,suffix:Hz"), "set_hf_reference", "get_hf_reference");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "hf_decay_ratio", PROPERTY_HINT_RANGE, "10,200,0.1,suffix:%"), "set_hf_decay_ratio", "get_hf_decay_ratio");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "diffusion", PROPERTY_HINT_RANGE, "0,100,0.1,suffix:%"), "set_diffusion", "get_diffusion");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "density", PROPERTY_HINT_RANGE, "0,100,0.1,suffix:%"), "set_density", "get_density");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "low_shelf_frequency", PROPERTY_HINT_RANGE, "20,1000,1,suffix:Hz"), "set_low_shelf_frequency", "get_low_shelf_frequency");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "low_shelf_gain", PROPERTY_HINT_RANGE, "-36,12,0.1,suffix:dB"), "set_low_shelf_gain", "get_low_shelf_gain");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "high_cut", PROPERTY_HINT_RANGE, "20,20000,1,suffix:Hz"), "set_high_cut", "get_high_cut");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "early_late_mix", PROPERTY_HINT_RANGE, "0,100,0.1,suffix:%"), "set_early_late_mix", "get_early_late_mix");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wet_level", PROPERTY_HINT_RANGE, "-80,20,0.1,suffix:dB"), "set_wet_level", "get_wet_level");
	}

	FmodReverbZone3D::FmodReverbZone3D() {
		set_process(true);
	}

	FmodReverbZone3D::~FmodReverbZone3D() {
		_release_reverb();
	}

	void FmodReverbZone3D::_notification(int p_what) {
		switch (p_what) {
			case NOTIFICATION_READY:
				_ensure_reverb();
				break;
			case NOTIFICATION_PROCESS:
				if (sync_transform && reverb.is_valid()) {
					_sync_reverb();
				}
				break;
			case NOTIFICATION_EXIT_TREE:
				_release_reverb();
				break;
		}
	}

	FMOD_REVERB_PROPERTIES FmodReverbZone3D::_preset_to_properties(Preset p_preset) {
		switch (p_preset) {
			case PRESET_OFF: return FMOD_REVERB_PROPERTIES FMOD_PRESET_OFF;
			case PRESET_PADDED_CELL: return FMOD_REVERB_PROPERTIES FMOD_PRESET_PADDEDCELL;
			case PRESET_ROOM: return FMOD_REVERB_PROPERTIES FMOD_PRESET_ROOM;
			case PRESET_BATHROOM: return FMOD_REVERB_PROPERTIES FMOD_PRESET_BATHROOM;
			case PRESET_LIVING_ROOM: return FMOD_REVERB_PROPERTIES FMOD_PRESET_LIVINGROOM;
			case PRESET_STONE_ROOM: return FMOD_REVERB_PROPERTIES FMOD_PRESET_STONEROOM;
			case PRESET_AUDITORIUM: return FMOD_REVERB_PROPERTIES FMOD_PRESET_AUDITORIUM;
			case PRESET_CONCERT_HALL: return FMOD_REVERB_PROPERTIES FMOD_PRESET_CONCERTHALL;
			case PRESET_CAVE: return FMOD_REVERB_PROPERTIES FMOD_PRESET_CAVE;
			case PRESET_ARENA: return FMOD_REVERB_PROPERTIES FMOD_PRESET_ARENA;
			case PRESET_HANGAR: return FMOD_REVERB_PROPERTIES FMOD_PRESET_HANGAR;
			case PRESET_HALLWAY: return FMOD_REVERB_PROPERTIES FMOD_PRESET_HALLWAY;
			case PRESET_STONE_CORRIDOR: return FMOD_REVERB_PROPERTIES FMOD_PRESET_STONECORRIDOR;
			case PRESET_ALLEY: return FMOD_REVERB_PROPERTIES FMOD_PRESET_ALLEY;
			case PRESET_FOREST: return FMOD_REVERB_PROPERTIES FMOD_PRESET_FOREST;
			case PRESET_CITY: return FMOD_REVERB_PROPERTIES FMOD_PRESET_CITY;
			case PRESET_MOUNTAINS: return FMOD_REVERB_PROPERTIES FMOD_PRESET_MOUNTAINS;
			case PRESET_UNDERWATER: return FMOD_REVERB_PROPERTIES FMOD_PRESET_UNDERWATER;
			case PRESET_GENERIC:
			case PRESET_CUSTOM:
			default:
				return FMOD_REVERB_PROPERTIES FMOD_PRESET_GENERIC;
		}
	}

	void FmodReverbZone3D::_ensure_reverb() {
		if (reverb.is_valid()) return;
		FmodServer* server = FmodServer::get_singleton();
		if (!server) return;
		Ref<FmodSystem> system = server->get_main_system();
		if (system.is_null()) return;
		reverb = system->create_reverb_3d();
		_sync_reverb();
	}

	void FmodReverbZone3D::_release_reverb() {
		if (reverb.is_valid()) {
			reverb->release();
			reverb.unref();
		}
	}

	Dictionary FmodReverbZone3D::_properties_to_dictionary() const {
		Dictionary dict;
		dict["decay_time"] = properties.DecayTime;
		dict["early_delay"] = properties.EarlyDelay;
		dict["late_delay"] = properties.LateDelay;
		dict["hf_reference"] = properties.HFReference;
		dict["hf_decay_ratio"] = properties.HFDecayRatio;
		dict["diffusion"] = properties.Diffusion;
		dict["density"] = properties.Density;
		dict["low_shelf_frequency"] = properties.LowShelfFrequency;
		dict["low_shelf_gain"] = properties.LowShelfGain;
		dict["high_cut"] = properties.HighCut;
		dict["early_late_mix"] = properties.EarlyLateMix;
		dict["wet_level"] = properties.WetLevel;
		return dict;
	}

	void FmodReverbZone3D::_sync_reverb() {
		if (!reverb.is_valid()) return;
		reverb->set_properties(_properties_to_dictionary());
		reverb->set_3d_attributes(get_global_position(), min_distance, max_distance);
		reverb->set_active(active);
	}

	void FmodReverbZone3D::_set_custom() {
		if (preset != PRESET_CUSTOM) {
			preset = PRESET_CUSTOM;
			notify_property_list_changed();
		}
	}

	void FmodReverbZone3D::set_active(bool p_active) {
		active = p_active;
		_sync_reverb();
	}

	bool FmodReverbZone3D::get_active() const {
		return active;
	}

	void FmodReverbZone3D::set_sync_transform(bool p_sync) {
		sync_transform = p_sync;
		set_process(sync_transform);
		_sync_reverb();
	}

	bool FmodReverbZone3D::get_sync_transform() const {
		return sync_transform;
	}

	void FmodReverbZone3D::set_min_distance(float p_distance) {
		min_distance = MAX(p_distance, 0.0f);
		max_distance = MAX(max_distance, min_distance + 0.001f);
		_sync_reverb();
		update_gizmos();
	}

	float FmodReverbZone3D::get_min_distance() const {
		return min_distance;
	}

	void FmodReverbZone3D::set_max_distance(float p_distance) {
		max_distance = MAX(p_distance, min_distance + 0.001f);
		_sync_reverb();
		update_gizmos();
	}

	float FmodReverbZone3D::get_max_distance() const {
		return max_distance;
	}

	void FmodReverbZone3D::set_preset(Preset p_preset) {
		preset = p_preset;
		if (preset != PRESET_CUSTOM) {
			properties = _preset_to_properties(preset);
		}
		_sync_reverb();
		notify_property_list_changed();
	}

	FmodReverbZone3D::Preset FmodReverbZone3D::get_preset() const {
		return preset;
	}

#define SET_REVERB_PROPERTY(m_setter, m_field, m_value, m_min, m_max) \
	properties.m_field = CLAMP(m_value, m_min, m_max);                \
	_set_custom();                                                    \
	_sync_reverb()

	void FmodReverbZone3D::set_decay_time(float p_value) { SET_REVERB_PROPERTY(set_decay_time, DecayTime, p_value, 100.0f, 20000.0f); }
	float FmodReverbZone3D::get_decay_time() const { return properties.DecayTime; }
	void FmodReverbZone3D::set_early_delay(float p_value) { SET_REVERB_PROPERTY(set_early_delay, EarlyDelay, p_value, 0.0f, 300.0f); }
	float FmodReverbZone3D::get_early_delay() const { return properties.EarlyDelay; }
	void FmodReverbZone3D::set_late_delay(float p_value) { SET_REVERB_PROPERTY(set_late_delay, LateDelay, p_value, 0.0f, 100.0f); }
	float FmodReverbZone3D::get_late_delay() const { return properties.LateDelay; }
	void FmodReverbZone3D::set_hf_reference(float p_value) { SET_REVERB_PROPERTY(set_hf_reference, HFReference, p_value, 20.0f, 20000.0f); }
	float FmodReverbZone3D::get_hf_reference() const { return properties.HFReference; }
	void FmodReverbZone3D::set_hf_decay_ratio(float p_value) { SET_REVERB_PROPERTY(set_hf_decay_ratio, HFDecayRatio, p_value, 10.0f, 200.0f); }
	float FmodReverbZone3D::get_hf_decay_ratio() const { return properties.HFDecayRatio; }
	void FmodReverbZone3D::set_diffusion(float p_value) { SET_REVERB_PROPERTY(set_diffusion, Diffusion, p_value, 0.0f, 100.0f); }
	float FmodReverbZone3D::get_diffusion() const { return properties.Diffusion; }
	void FmodReverbZone3D::set_density(float p_value) { SET_REVERB_PROPERTY(set_density, Density, p_value, 0.0f, 100.0f); }
	float FmodReverbZone3D::get_density() const { return properties.Density; }
	void FmodReverbZone3D::set_low_shelf_frequency(float p_value) { SET_REVERB_PROPERTY(set_low_shelf_frequency, LowShelfFrequency, p_value, 20.0f, 1000.0f); }
	float FmodReverbZone3D::get_low_shelf_frequency() const { return properties.LowShelfFrequency; }
	void FmodReverbZone3D::set_low_shelf_gain(float p_value) { SET_REVERB_PROPERTY(set_low_shelf_gain, LowShelfGain, p_value, -36.0f, 12.0f); }
	float FmodReverbZone3D::get_low_shelf_gain() const { return properties.LowShelfGain; }
	void FmodReverbZone3D::set_high_cut(float p_value) { SET_REVERB_PROPERTY(set_high_cut, HighCut, p_value, 20.0f, 20000.0f); }
	float FmodReverbZone3D::get_high_cut() const { return properties.HighCut; }
	void FmodReverbZone3D::set_early_late_mix(float p_value) { SET_REVERB_PROPERTY(set_early_late_mix, EarlyLateMix, p_value, 0.0f, 100.0f); }
	float FmodReverbZone3D::get_early_late_mix() const { return properties.EarlyLateMix; }
	void FmodReverbZone3D::set_wet_level(float p_value) { SET_REVERB_PROPERTY(set_wet_level, WetLevel, p_value, -80.0f, 20.0f); }
	float FmodReverbZone3D::get_wet_level() const { return properties.WetLevel; }

#undef SET_REVERB_PROPERTY

	Ref<FmodReverb3D> FmodReverbZone3D::get_reverb() const {
		return reverb;
	}
}
