#ifndef FMOD_SOUND_LOCK_H
#define FMOD_SOUND_LOCK_H

#include <fmod.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
	class FmodSoundLock : public RefCounted {
		GDCLASS(FmodSoundLock, RefCounted)

	private:
		FMOD::Sound* sound;
		void* ptr1;
		void* ptr2;
		unsigned int len1;
		unsigned int len2;
		bool locked;

	protected:
		static void _bind_methods();

	public:
		FmodSoundLock() : sound(nullptr), ptr1(nullptr), ptr2(nullptr),
			len1(0), len2(0), locked(false) {}

		~FmodSoundLock() {
			unlock();
		}

		bool _lock(FMOD::Sound* p_sound, unsigned int offset, unsigned int length);
		void unlock();
		PackedByteArray get_data1() const;
		PackedByteArray get_data2() const;
		bool is_locked() const;
	};
}

#endif // !FMOD_SOUND_LOCK_H
