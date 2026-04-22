#include "fmod_reverb_3d.h"

namespace godot {
	void FmodReverb3D::_bind_methods() {
		ClassDB::bind_method(D_METHOD("reverb_3d_is_valid"), &FmodReverb3D::reverb_3d_is_valid);
		ClassDB::bind_method(D_METHOD("reverb_3d_is_null"), &FmodReverb3D::reverb_3d_is_null);

		ClassDB::bind_method(D_METHOD("set_3d_attributes", "position", "min_distance", "max_distance"), &FmodReverb3D::set_3d_attributes);
		ClassDB::bind_method(D_METHOD("get_3d_attributes"), &FmodReverb3D::get_3d_attributes);

		ClassDB::bind_method(D_METHOD("set_active", "active"), &FmodReverb3D::set_active);
		ClassDB::bind_method(D_METHOD("get_active"), &FmodReverb3D::get_active);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");

		ClassDB::bind_method(D_METHOD("release"), &FmodReverb3D::release);

		ClassDB::bind_method(D_METHOD("set_decay_time", "decay_time"), &FmodReverb3D::set_decay_time);
		ClassDB::bind_method(D_METHOD("get_decay_time"), &FmodReverb3D::get_decay_time);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "decay_time", PROPERTY_HINT_RANGE, "0,1500,0.1,suffix:ms"), "set_decay_time", "get_decay_time");

		ClassDB::bind_method(D_METHOD("set_early_delay", "early_delay"), &FmodReverb3D::set_early_delay);
		ClassDB::bind_method(D_METHOD("get_early_delay"), &FmodReverb3D::get_early_delay);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "early_delay", PROPERTY_HINT_RANGE, "0,300,0.1,suffix:ms"), "set_early_delay", "get_early_delay");

		ClassDB::bind_method(D_METHOD("set_late_delay", "late_delay"), &FmodReverb3D::set_late_delay);
		ClassDB::bind_method(D_METHOD("get_late_delay"), &FmodReverb3D::get_late_delay);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "late_delay", PROPERTY_HINT_RANGE, "0,100,0.1,suffix:ms"), "set_late_delay", "get_late_delay");

		ClassDB::bind_method(D_METHOD("set_hf_reference", "hf_reference"), &FmodReverb3D::set_hf_reference);
		ClassDB::bind_method(D_METHOD("get_hf_reference"), &FmodReverb3D::get_hf_reference);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "hf_reference", PROPERTY_HINT_RANGE, "20,20000,0.1,suffix:Hz"), "set_hf_reference", "get_hf_reference");

		ClassDB::bind_method(D_METHOD("set_hf_decay_ratio", "hf_decay_ratio"), &FmodReverb3D::set_hf_decay_ratio);
		ClassDB::bind_method(D_METHOD("get_hf_decay_ratio"), &FmodReverb3D::get_hf_decay_ratio);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "hf_decay_ratio", PROPERTY_HINT_RANGE, "10,100,0.1"), "set_hf_decay_ratio", "get_hf_decay_ratio");

		ClassDB::bind_method(D_METHOD("set_diffusion", "diffusion"), &FmodReverb3D::set_diffusion);
		ClassDB::bind_method(D_METHOD("get_diffusion"), &FmodReverb3D::get_diffusion);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "diffusion", PROPERTY_HINT_RANGE, "10,100,0.1"), "set_diffusion", "get_diffusion");

		ClassDB::bind_method(D_METHOD("set_density", "density"), &FmodReverb3D::set_density);
		ClassDB::bind_method(D_METHOD("get_density"), &FmodReverb3D::get_density);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "density", PROPERTY_HINT_RANGE, "0,100,0.1"), "set_density", "get_density");

		ClassDB::bind_method(D_METHOD("set_low_shelf_frequency", "low_shelf_frequency"), &FmodReverb3D::set_low_shelf_frequency);
		ClassDB::bind_method(D_METHOD("get_low_shelf_frequency"), &FmodReverb3D::get_low_shelf_frequency);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "low_shelf_frequency", PROPERTY_HINT_RANGE, "20,1000,0.1,suffix:Hz"), "set_low_shelf_frequency", "get_low_shelf_frequency");

		ClassDB::bind_method(D_METHOD("set_low_shelf_gain", "low_shelf_gain"), &FmodReverb3D::set_low_shelf_gain);
		ClassDB::bind_method(D_METHOD("get_low_shelf_gain"), &FmodReverb3D::get_low_shelf_gain);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "low_shelf_gain", PROPERTY_HINT_RANGE, "-36,12,0.1,suffix:Db"), "set_low_shelf_gain", "get_low_shelf_gain");

		ClassDB::bind_method(D_METHOD("set_high_cut", "high_cut"), &FmodReverb3D::set_high_cut);
		ClassDB::bind_method(D_METHOD("get_high_cut"), &FmodReverb3D::get_high_cut);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "high_cut", PROPERTY_HINT_RANGE, "0,20000,0.1,suffix:Hz"), "set_high_cut", "get_high_cut");

		ClassDB::bind_method(D_METHOD("set_early_late_mix", "early_late_mix"), &FmodReverb3D::set_early_late_mix);
		ClassDB::bind_method(D_METHOD("get_early_late_mix"), &FmodReverb3D::get_early_late_mix);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "early_late_mix", PROPERTY_HINT_RANGE, "0,100,0.1"), "set_early_late_mix", "get_early_late_mix");

		ClassDB::bind_method(D_METHOD("set_wet_level", "wet_level"), &FmodReverb3D::set_wet_level);
		ClassDB::bind_method(D_METHOD("get_wet_level"), &FmodReverb3D::get_wet_level);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wet_level", PROPERTY_HINT_RANGE, "-80,20,0.1,suffix:Db"), "set_wet_level", "get_wet_level");
	}

	FmodReverb3D::FmodReverb3D() {

	}

	void FmodReverb3D::_apply_properties() {
		ERR_FAIL_COND(!reverb_3d);
		FMOD_ERR_CHECK(reverb_3d->setProperties(&properties));
	}

	FmodReverb3D::~FmodReverb3D() {
		if (reverb_3d) {
			reverb_3d->setUserData(nullptr);
			reverb_3d = nullptr;
		}
	}

	bool FmodReverb3D::reverb_3d_is_valid() const {
		return reverb_3d != nullptr;
	}

	bool FmodReverb3D::reverb_3d_is_null() const {
		return reverb_3d == nullptr;
	}

	void FmodReverb3D::setup(FMOD::Reverb3D* p_reverb_3d) {
		ERR_FAIL_COND_MSG(!p_reverb_3d, "DSP pointer is null");

		if (reverb_3d) {
			reverb_3d->setUserData(nullptr);
		}

		reverb_3d = p_reverb_3d;
		reverb_3d->setUserData(this);
	}

	void FmodReverb3D::set_3d_attributes(const Vector3 position, const float min_distance, const float max_distance) {
		ERR_FAIL_COND(!reverb_3d);
		FMOD_VECTOR fmod_position = { position.x, position.y, position.z };
		FMOD_ERR_CHECK(reverb_3d->set3DAttributes(&fmod_position, min_distance, max_distance));
	}

	Dictionary FmodReverb3D::get_3d_attributes() const {
		ERR_FAIL_COND_V(!reverb_3d, Dictionary());
		FMOD_VECTOR fmod_position = {};
		float min_distance = 0.0f, max_distance = 0.0f;
		FMOD_ERR_CHECK_V(reverb_3d->get3DAttributes(&fmod_position, &min_distance, &max_distance), Dictionary());
		Dictionary result;
		result["position"] = Vector3(fmod_position.x, fmod_position.y, fmod_position.z);
		result["min_distance"] = min_distance;
		result["max_distance"] = max_distance;
		return result;
	}

	void FmodReverb3D::set_active(const bool active) {
		ERR_FAIL_COND(!reverb_3d);
		FMOD_ERR_CHECK(reverb_3d->setActive(active));
	}

	bool FmodReverb3D::get_active() const {
		if (!reverb_3d) return false;
		bool active = false;
		FMOD_ERR_CHECK_V(reverb_3d->getActive(&active), false);
		return active;
	}

	void FmodReverb3D::release() {
		ERR_FAIL_COND(!reverb_3d);
		reverb_3d->setUserData(nullptr);
		FMOD_RESULT result = reverb_3d->release();
		if (result != FMOD_OK) {
			ERR_PRINT("Failed to release Reverb3D");
			return;
		}
		reverb_3d = nullptr;
	}

	void FmodReverb3D::set_decay_time(const float decay_time) {
		properties.DecayTime = decay_time;
		_apply_properties();
	}

	float FmodReverb3D::get_decay_time() const {
		return properties.DecayTime;
	}

	void FmodReverb3D::set_early_delay(const float early_delay) {
		properties.EarlyDelay = early_delay;
		_apply_properties();
	}

	float FmodReverb3D::get_early_delay() const {
		return properties.EarlyDelay;
	}

	void FmodReverb3D::set_late_delay(const float late_delay) {
		properties.LateDelay = late_delay;
		_apply_properties();
	}

	float FmodReverb3D::get_late_delay() const {
		return properties.LateDelay;
	}

	void FmodReverb3D::set_hf_reference(const float hf_reference) {
		properties.HFReference = hf_reference;
		_apply_properties();
	}

	float FmodReverb3D::get_hf_reference() const {
		return properties.HFReference;
	}

	void FmodReverb3D::set_hf_decay_ratio(const float hf_decay_ratio) {
		properties.HFDecayRatio = hf_decay_ratio;
		_apply_properties();
	}

	float FmodReverb3D::get_hf_decay_ratio() const {
		return properties.HFDecayRatio;
	}

	void FmodReverb3D::set_diffusion(const float diffusion) {
		properties.Diffusion = diffusion;
		_apply_properties();
	}

	float FmodReverb3D::get_diffusion() const {
		return properties.Diffusion;
	}

	void FmodReverb3D::set_density(const float density) {
		properties.Density = density;
		_apply_properties();
	}

	float FmodReverb3D::get_density() const {
		return properties.Density;
	}

	void FmodReverb3D::set_low_shelf_frequency(const float low_shelf_frequency) {
		properties.LowShelfFrequency = low_shelf_frequency;
		_apply_properties();
	}

	float FmodReverb3D::get_low_shelf_frequency() const {
		return properties.LowShelfFrequency;
	}

	void FmodReverb3D::set_low_shelf_gain(const float low_shelf_gain) {
		properties.LowShelfGain = low_shelf_gain;
		_apply_properties();
	}

	float FmodReverb3D::get_low_shelf_gain() const {
		return properties.LowShelfGain;
	}

	void FmodReverb3D::set_high_cut(const float high_cut) {
		properties.HighCut = high_cut;
		_apply_properties();
	}

	float FmodReverb3D::get_high_cut() const {
		return properties.HighCut;
	}

	void FmodReverb3D::set_early_late_mix(const float early_late_mix) {
		properties.EarlyLateMix = early_late_mix;
		_apply_properties();
	}

	float FmodReverb3D::get_early_late_mix() const {
		return properties.EarlyLateMix;
	}

	void FmodReverb3D::set_wet_level(const float wet_level) {
		properties.WetLevel = wet_level;
		_apply_properties();
	}

	float FmodReverb3D::get_wet_level() const {
		return properties.WetLevel;
	}
}