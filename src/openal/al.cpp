#include "al.hpp"
#include "alc.h"
#include "al.h"
#include <exception>

#define alc_device_ptr (static_cast<ALCdevice*>(alc_device_ptr_))

al::device&& al::open_device(std::string device_name) {
	ALCdevice* device = alcOpenDevice(device_name.length() == 0 ? nullptr : device_name.c_str());
	if (!device) {
		throw std::exception("Error when opening AL device");
	}
	return device;
}

al::context&& al::device::create_context() {
	return alcCreateContext(alc_device_ptr, nullptr);
}

void al::make_context_current(context& c) {
	alcMakeContextCurrent(static_cast<ALCcontext*>(c.alc_context_ptr_));
}

unsigned al::get_error() {
	return alGetError();
}

/* Buffer */
void al::buffer::al_gen(int n, unsigned* buffers) {
	alGenBuffers(1, buffers);
}
void al::buffer::al_data(unsigned name, unsigned format, void* data, int size, int frequency) {
	alBufferData(name, format, data, size, frequency);
}

/* Source */
void al::source::al_gen(int n, unsigned* sources) {
	alGenSources(n, sources);
}

void al::source::al_i(unsigned source, unsigned param, int value) {
	alSourcei(source, param, value);
}

void al::source::play() {
	alSourcePlay(name);
}