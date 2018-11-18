#include "al.hpp"
#include "alc.h"
#include "al.h"

#define alc_device_ptr (static_cast<ALCdevice*>(alc_device_ptr_))

unsigned al::buffer::al_gen() {
	unsigned name;
	alGenBuffers(1, &name);
	return name;
}

al::device&& al::open_device(std::string device_name) {
	return alcOpenDevice(device_name.length() == 0 ? nullptr : device_name.c_str());
}

al::context&& al::device::create_context() {
	return alcCreateContext(alc_device_ptr, nullptr);
}