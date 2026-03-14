#include "fmod_sound.h"

#include <cstring>

namespace godot {
	void FmodSound::_bind_methods() {
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_UNKNOWN);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_AIFF);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_ASF);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_DLS);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_FLAC);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_FSB);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_IT);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_MIDI);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_MOD);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_MPEG);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_OGGVORBIS);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_PLAYLIST);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_RAW);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_S3M);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_USER);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_WAV);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_XM);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_XMA);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_AUDIOQUEUE);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_AT9);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_VORBIS);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_MEDIA_FOUNDATION);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_MEDIACODEC);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_FADPCM);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_OPUS);
		BIND_ENUM_CONSTANT(FMOD_SOUND_TYPE_MAX);

		BIND_ENUM_CONSTANT(FMOD_SOUND_FORMAT_NONE);
		BIND_ENUM_CONSTANT(FMOD_SOUND_FORMAT_PCM8);
		BIND_ENUM_CONSTANT(FMOD_SOUND_FORMAT_PCM16);
		BIND_ENUM_CONSTANT(FMOD_SOUND_FORMAT_PCM24);
		BIND_ENUM_CONSTANT(FMOD_SOUND_FORMAT_PCM32);
		BIND_ENUM_CONSTANT(FMOD_SOUND_FORMAT_PCMFLOAT);
		BIND_ENUM_CONSTANT(FMOD_SOUND_FORMAT_BITSTREAM);
		BIND_ENUM_CONSTANT(FMOD_SOUND_FORMAT_MAX);

		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_UNKNOWN);
		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_ID3V1);
		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_ID3V2);
		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_VORBISCOMMENT);
		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_SHOUTCAST);
		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_ICECAST);
		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_ASF);
		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_MIDI);
		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_PLAYLIST);
		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_FMOD);
		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_USER);
		BIND_ENUM_CONSTANT(FMOD_TAG_TYPE_MAX);

		BIND_ENUM_CONSTANT(FMOD_TAG_DATA_TYPE_BINARY);
		BIND_ENUM_CONSTANT(FMOD_TAG_DATA_TYPE_INT);
		BIND_ENUM_CONSTANT(FMOD_TAG_DATA_TYPE_FLOAT);
		BIND_ENUM_CONSTANT(FMOD_TAG_DATA_TYPE_STRING);
		BIND_ENUM_CONSTANT(FMOD_TAG_DATA_TYPE_STRING_UTF16);
		BIND_ENUM_CONSTANT(FMOD_TAG_DATA_TYPE_STRING_UTF16BE);
		BIND_ENUM_CONSTANT(FMOD_TAG_DATA_TYPE_STRING_UTF8);
		BIND_ENUM_CONSTANT(FMOD_TAG_DATA_TYPE_MAX);

		BIND_ENUM_CONSTANT(FMOD_OPEN_STATE_READY);
		BIND_ENUM_CONSTANT(FMOD_OPEN_STATE_LOADING);
		BIND_ENUM_CONSTANT(FMOD_OPEN_STATE_ERROR);
		BIND_ENUM_CONSTANT(FMOD_OPEN_STATE_CONNECTING);
		BIND_ENUM_CONSTANT(FMOD_OPEN_STATE_BUFFERING);
		BIND_ENUM_CONSTANT(FMOD_OPEN_STATE_SEEKING);
		BIND_ENUM_CONSTANT(FMOD_OPEN_STATE_PLAYING);
		BIND_ENUM_CONSTANT(FMOD_OPEN_STATE_SETPOSITION);
		BIND_ENUM_CONSTANT(FMOD_OPEN_STATE_MAX);

		ClassDB::bind_static_method("FmodSound", D_METHOD("load_from_file", "path"), &FmodSound::load_from_file);

		ClassDB::bind_method(D_METHOD("sound_is_valid"), &FmodSound::sound_is_valid);
		ClassDB::bind_method(D_METHOD("sound_is_null"), &FmodSound::sound_is_null);

		ClassDB::bind_method(D_METHOD("get_name"), &FmodSound::get_name);
		ClassDB::bind_method(D_METHOD("get_format"), &FmodSound::get_format);
		ClassDB::bind_method(D_METHOD("get_length", "time_unit"), &FmodSound::get_length, DEFVAL(FmodSystem::FMOD_TIME_UNIT_MS));
		ClassDB::bind_method(D_METHOD("get_num_tags"), &FmodSound::get_num_tags);
		ClassDB::bind_method(D_METHOD("get_tag", "index", "name"), &FmodSound::get_tag, DEFVAL(String()));

		ClassDB::bind_method(D_METHOD("get_open_state"), &FmodSound::get_open_state);
		ClassDB::bind_method(D_METHOD("read_data", "length"), &FmodSound::read_data);
		ClassDB::bind_method(D_METHOD("seek_data", "pcm"), &FmodSound::seek_data);
		ClassDB::bind_method(D_METHOD("lock", "offset", "length"), &FmodSound::lock);

		ClassDB::bind_method(D_METHOD("get_music_num_channels"), &FmodSound::get_music_num_channels);
		ClassDB::bind_method(D_METHOD("set_music_channel_volume", "channel", "volume_db"), &FmodSound::set_music_channel_volume);
		ClassDB::bind_method(D_METHOD("get_music_channel_volume"), &FmodSound::get_music_channel_volume);
		ClassDB::bind_method(D_METHOD("set_music_speed", "speed"), &FmodSound::set_music_speed);
		ClassDB::bind_method(D_METHOD("get_music_speed"), &FmodSound::get_music_speed);

		ClassDB::bind_method(D_METHOD("get_sync_point", "index"), &FmodSound::get_sync_point);
		ClassDB::bind_method(D_METHOD("get_sync_point_info", "point", "time_unit"), &FmodSound::get_sync_point_info, DEFVAL(FmodSystem::FMOD_TIME_UNIT_MS));
		ClassDB::bind_method(D_METHOD("get_num_sub_sounds"), &FmodSound::get_num_sub_sounds);

		// Callback setters/getters
		ClassDB::bind_method(D_METHOD("set_pcmread_callback", "callback"), &FmodSound::set_pcmread_callback);
		ClassDB::bind_method(D_METHOD("get_pcmread_callback"), &FmodSound::get_pcmread_callback);
		ClassDB::bind_method(D_METHOD("set_pcmsetpos_callback", "callback"), &FmodSound::set_pcmsetpos_callback);
		ClassDB::bind_method(D_METHOD("get_pcmsetpos_callback"), &FmodSound::get_pcmsetpos_callback);
		ClassDB::bind_method(D_METHOD("set_nonblock_callback", "callback"), &FmodSound::set_nonblock_callback);
		ClassDB::bind_method(D_METHOD("get_nonblock_callback"), &FmodSound::get_nonblock_callback);
	}

	FmodSound::FmodSound() {

	}

	FmodSound::~FmodSound() {
		if (sound) {
			sound = nullptr;
		}
	}

	Ref<FmodSound> FmodSound::load_from_file(const String& p_path) {
		return FmodServer::get_main_system()->create_sound_from_file(p_path, FmodSystem::FMOD_MODE_DEFAULT);
	}

	bool FmodSound::sound_is_valid() const {
		return sound != nullptr;
	}

	bool FmodSound::sound_is_null() const {
		return sound == nullptr;
	}

	void FmodSound::setup(FMOD::Sound* p_sound) {
		ERR_FAIL_COND_MSG(!p_sound, "Sound pointer is null");

		if (sound) {
			sound->setUserData(nullptr);
		}

		sound = p_sound;
		sound->setUserData(this);
	}

	String FmodSound::get_name() const {
		ERR_FAIL_COND_V(!sound, String());
		char name[512] = { 0 };
		FMOD_ERR_CHECK(sound->getName(name, 512));
		return String::utf8(name);
	}

	Dictionary FmodSound::get_format() const {
		ERR_FAIL_COND_V(!sound, Dictionary());
		FMOD_SOUND_TYPE type;
		FMOD_SOUND_FORMAT format;
		int channels;
		int bits;
		FMOD_ERR_CHECK_V(sound->getFormat(&type, &format, &channels, &bits), Dictionary());
		Dictionary info;
		info["type"] = (int)type;
		info["format"] = (int)format;
		info["channels"] = channels;
		info["bits"] = bits;
		return info;
	}

	double FmodSound::get_length(FmodSystem::FmodTimeUnit time_unit) const {
		unsigned int length = 0;
		FMOD_ERR_CHECK(sound->getLength(&length, time_unit));
		// 只有时间单位（MS）才需要转换为秒，字节数和采样数直接返回
		if (time_unit == FmodSystem::FMOD_TIME_UNIT_MS) {
			return (double)length / 1000.0;
		}
		return (double)length;
	}

	Dictionary FmodSound::get_num_tags() const {
		ERR_FAIL_COND_V(!sound, Dictionary());
		int numtags = 0;
		int numtagsupdated = 0;
		FMOD_ERR_CHECK_V(sound->getNumTags(&numtags, &numtagsupdated), Dictionary());
		Dictionary result;
		result["num_tags"] = numtags;
		result["num_tags_updated"] = numtagsupdated;
		return result;
	}

	Dictionary FmodSound::get_tag(const int index, const String& name) const {
		ERR_FAIL_COND_V(!sound, Dictionary());
		const char* char_ptr = nullptr;
		if (!name.is_empty()) {
			char_ptr = name.utf8().get_data();
		}
		FMOD_TAG tag = {};
		FMOD_ERR_CHECK_V(sound->getTag(char_ptr, index, &tag), Dictionary());
		Dictionary info;
		info["type"] = (int)tag.type;
		info["datatype"] = (int)tag.datatype;
		info["name"] = String::utf8(tag.name);
		switch (tag.datatype) {
		case FMOD_TAGDATATYPE_BINARY: {
			// void* → PackedByteArray
			PackedByteArray data;
			data.resize(tag.datalen);
			if (tag.data != nullptr && tag.datalen > 0) {
				memcpy(data.ptrw(), tag.data, tag.datalen);
			}
			info["data"] = data;
		} break;

		case FMOD_TAGDATATYPE_INT: {
			// void* → int (根据文档，可能是 int 或 long)
			if (tag.datalen == sizeof(int)) {
				info["data"] = *(int*)tag.data;
			}
			else if (tag.datalen == sizeof(long)) {
				info["data"] = (int)(*(long*)tag.data);
			}
		} break;

		case FMOD_TAGDATATYPE_FLOAT: {
			// void* → float
			if (tag.datalen == sizeof(float)) {
				info["data"] = *(float*)tag.data;
			}
			else if (tag.datalen == sizeof(double)) {
				info["data"] = (float)(*(double*)tag.data);
			}
		} break;

		case FMOD_TAGDATATYPE_STRING: {
			// void* → String (UTF-8 字符串)
			if (tag.data != nullptr && tag.datalen > 0) {
				// 确保字符串以 null 结尾
				const char* str_data = static_cast<const char*>(tag.data);
				// 使用 UTF-8 转换
				info["data"] = String::utf8(str_data, tag.datalen);
			}
			else {
				info["data"] = String();
			}
		} break;

		case FMOD_TAGDATATYPE_STRING_UTF16: {
			// void* → String (UTF-16 字符串)
			if (tag.data != nullptr && tag.datalen > 0) {
				// UTF-16 需要特殊处理
				const char16_t* utf16_data = static_cast<const char16_t*>(tag.data);
				int len = tag.datalen / sizeof(char16_t);
				info["data"] = String::utf16(utf16_data, len);
			}
			else {
				info["data"] = String();
			}
			break;
		}

		case FMOD_TAGDATATYPE_STRING_UTF16BE: {
			// UTF-16BE (大端序) - 需要转换
			if (tag.data != nullptr && tag.datalen > 0) {
				// 简单处理：先转为 UTF-8
				const uint8_t* bytes = static_cast<const uint8_t*>(tag.data);
				// 这里需要实现 UTF-16BE 到 UTF-8 的转换
				// 为简化，先存为二进制
				PackedByteArray data;
				data.resize(tag.datalen);
				memcpy(data.ptrw(), bytes, tag.datalen);
				info["data"] = data;
			}
			break;
		}

		case FMOD_TAGDATATYPE_STRING_UTF8: {
			// void* → String (明确指定 UTF-8)
			if (tag.data != nullptr && tag.datalen > 0) {
				info["data"] = String::utf8(static_cast<const char*>(tag.data), tag.datalen);
			}
			else {
				info["data"] = String();
			}
			break;
		}
		}
		info["data_len"] = tag.datalen;
		info["updated"] = tag.updated == 1;
		return info;
	}

	Dictionary FmodSound::get_open_state() const {
		ERR_FAIL_COND_V(!sound, Dictionary());
		FMOD_OPENSTATE openstate = FMOD_OPENSTATE_ERROR;
		unsigned int percentbuffered = 0;
		bool starving = false, diskbusy = false;
		FMOD_ERR_CHECK_V(sound->getOpenState(&openstate, &percentbuffered, &starving, &diskbusy), Dictionary());
		Dictionary state;
		state["open_state"] = (int)openstate;
		state["percent_buffered"] = percentbuffered;
		state["starving"] = starving;
		state["disk_busy"] = diskbusy;
		return state;
	}

	PackedByteArray FmodSound::read_data(const unsigned int length) const {
		ERR_FAIL_COND_V(!sound, PackedByteArray());
		// 分配缓冲区
		PackedByteArray data;
		data.resize(length);

		// 获取可写指针
		void* buffer = data.ptrw();
		unsigned int read = 0;

		// 读取数据到缓冲区
		FMOD_ERR_CHECK_V(sound->readData(buffer, length, &read), PackedByteArray());

		// 调整大小为实际读取的数据量
		data.resize(read);
		return data;
	}

	void FmodSound::seek_data(const unsigned int pcm) {
		ERR_FAIL_COND(!sound);
		FMOD_ERR_CHECK(sound->seekData(pcm));
	}

	Ref<FmodSoundLock> FmodSound::lock(const unsigned int offset, const unsigned int length) const {
		ERR_FAIL_COND_V(!sound, Ref<FmodSoundLock>());
		Ref<FmodSoundLock> lock;
		lock.instantiate();

		if (!lock->_lock(sound, offset, length)) {
			return Ref<FmodSoundLock>();
		}

		return lock;
	}

	int FmodSound::get_music_num_channels() const {
		ERR_FAIL_COND_V(!sound, 0);
		int numchannels = 0;
		FMOD_ERR_CHECK_V(sound->getMusicNumChannels(&numchannels), 0);
		return numchannels;
	}

	void FmodSound::set_music_channel_volume(const int channel, const float volume_db) const {
		ERR_FAIL_COND(!sound);
		FMOD_ERR_CHECK(sound->setMusicChannelVolume(channel, FmodUtils::db_to_linear(volume_db)));
	}

	float FmodSound::get_music_channel_volume(const int channel) const {
		ERR_FAIL_COND_V(!sound, 0.0f);
		float volume = 0.0f;
		FMOD_ERR_CHECK_V(sound->getMusicChannelVolume(channel, &volume), 0.0f);
		return FmodUtils::linear_to_db(volume);
	}

	void FmodSound::set_music_speed(const float speed) {
		ERR_FAIL_COND(!sound);
		FMOD_ERR_CHECK(sound->setMusicSpeed(speed));
	}

	float FmodSound::get_music_speed() const {
		ERR_FAIL_COND_V(!sound, 0.0f);
		float speed = 0.0f;
		FMOD_ERR_CHECK_V(sound->getMusicSpeed(&speed), 0.0f);
		return speed;
	}

	int64_t FmodSound::get_sync_point(const int index) const {
		ERR_FAIL_COND_V(!sound, 0);
		FMOD_SYNCPOINT* point = nullptr;
		FMOD_ERR_CHECK_V(sound->getSyncPoint(index, &point), 0);
		return (int64_t)(uintptr_t)point;
	}

	Dictionary FmodSound::get_sync_point_info(const int64_t point_ptr, FmodSystem::FmodTimeUnit time_unit) const {
		ERR_FAIL_COND_V(!sound, Dictionary());

		FMOD_SYNCPOINT* point = reinterpret_cast<FMOD_SYNCPOINT*>(static_cast<uintptr_t>(point_ptr));
		char name[512] = { 0 };
		unsigned int offset = 0;

		FMOD_TIMEUNIT fmod_time_unit = static_cast<FMOD_TIMEUNIT>((int)time_unit);
		FMOD_RESULT result = sound->getSyncPointInfo(point, name, 512, &offset, fmod_time_unit);
		FMOD_ERR_CHECK_V(result, Dictionary());

		Dictionary info;
		info["name"] = String(name);
		info["offset"] = offset;
		info["time_unit"] = time_unit;
		info["pointer"] = point_ptr;

		return info;
	}

	int FmodSound::get_num_sub_sounds() const {
		ERR_FAIL_COND_V(!sound, 0);
		int numsubsounds = 0;
		FMOD_ERR_CHECK_V(sound->getNumSubSounds(&numsubsounds), 0);
		return numsubsounds;
	}

	// Callback implementations
	void FmodSound::set_pcmread_callback(const Callable &p_callback) {
		_pcmread_callback = p_callback;
	}

	Callable FmodSound::get_pcmread_callback() const {
		return _pcmread_callback;
	}

	FMOD_RESULT FmodSound::_handle_pcmread_callback(void *data, unsigned int datalen) const {
		if (_pcmread_callback.is_valid()) {
			// 将 PCM 数据包装为 PackedByteArray
			PackedByteArray pcm_data;
			pcm_data.resize(datalen);
			memcpy(pcm_data.ptrw(), data, datalen);
			
			const Variant ret = _pcmread_callback.call(pcm_data);
			if (ret.get_type() == Variant::INT) {
				return static_cast<FMOD_RESULT>(int(ret));
			}
		}
		return FMOD_OK;
	}

	void FmodSound::set_pcmsetpos_callback(const Callable &p_callback) {
		_pcmsetpos_callback = p_callback;
	}

	Callable FmodSound::get_pcmsetpos_callback() const {
		return _pcmsetpos_callback;
	}

	FMOD_RESULT FmodSound::_handle_pcmsetpos_callback(int subsound, unsigned int position, FMOD_TIMEUNIT postype) const {
		if (_pcmsetpos_callback.is_valid()) {
			const Variant ret = _pcmsetpos_callback.call(subsound, position, static_cast<int>(postype));
			if (ret.get_type() == Variant::INT) {
				return static_cast<FMOD_RESULT>(int(ret));
			}
		}
		return FMOD_OK;
	}

	void FmodSound::set_nonblock_callback(const Callable &p_callback) {
		_nonblock_callback = p_callback;
	}

	Callable FmodSound::get_nonblock_callback() const {
		return _nonblock_callback;
	}

	FMOD_RESULT FmodSound::_handle_nonblock_callback(FMOD_RESULT result) const {
		if (_nonblock_callback.is_valid()) {
			const Variant ret = _nonblock_callback.call(static_cast<int>(result));
			if (ret.get_type() == Variant::INT) {
				return static_cast<FMOD_RESULT>(int(ret));
			}
		}
		return FMOD_OK;
	}
}

// C 回调函数实现
// 注意：FMOD_SOUND 是 C API 类型，需要转换为 FMOD::Sound (C++ API) 使用
FMOD_RESULT F_CALL GD_FMOD_SOUND_PCMREAD_CALLBACK(
	FMOD_SOUND* sound,
	void* data,
	unsigned int datalen
) {
	if (!sound || !data) {
		return FMOD_ERR_INVALID_PARAM;
	}
	
	// 将 FMOD_SOUND* 转换为 FMOD::Sound*
	FMOD::Sound* fmod_sound = reinterpret_cast<FMOD::Sound*>(sound);
	
	void* user_data = nullptr;
	FMOD_RESULT result = fmod_sound->getUserData(&user_data);
	if (result != FMOD_OK || !user_data) {
		return FMOD_OK;
	}
	
	return static_cast<godot::FmodSound*>(user_data)->_handle_pcmread_callback(data, datalen);
}

FMOD_RESULT F_CALL GD_FMOD_SOUND_PCMSETPOS_CALLBACK(
	FMOD_SOUND* sound,
	int subsound,
	unsigned int position,
	FMOD_TIMEUNIT postype
) {
	if (!sound) {
		return FMOD_ERR_INVALID_PARAM;
	}
	
	// 将 FMOD_SOUND* 转换为 FMOD::Sound*
	FMOD::Sound* fmod_sound = reinterpret_cast<FMOD::Sound*>(sound);
	
	void* user_data = nullptr;
	FMOD_RESULT result = fmod_sound->getUserData(&user_data);
	if (result != FMOD_OK || !user_data) {
		return FMOD_OK;
	}
	
	return static_cast<godot::FmodSound*>(user_data)->_handle_pcmsetpos_callback(subsound, position, postype);
}

FMOD_RESULT F_CALL GD_FMOD_SOUND_NONBLOCK_CALLBACK(
	FMOD_SOUND* sound,
	FMOD_RESULT result
) {
	if (!sound) {
		return FMOD_ERR_INVALID_PARAM;
	}
	
	// 将 FMOD_SOUND* 转换为 FMOD::Sound*
	FMOD::Sound* fmod_sound = reinterpret_cast<FMOD::Sound*>(sound);
	
	void* user_data = nullptr;
	FMOD_RESULT get_result = fmod_sound->getUserData(&user_data);
	if (get_result != FMOD_OK || !user_data) {
		return FMOD_OK;
	}
	
	return static_cast<godot::FmodSound*>(user_data)->_handle_nonblock_callback(result);
}