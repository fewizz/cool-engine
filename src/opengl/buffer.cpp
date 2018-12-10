#include "buffer.hpp"
#include "GL/glew.h"

void gl::internal::bind_buffer(unsigned target, unsigned buffer) {
	glBindBuffer(target, buffer);
}

void gl::internal::gen_buffers(unsigned n, unsigned* buffers) {
	glGenBuffers(n, buffers);
}

void gl::internal::delete_buffers(unsigned n, unsigned* buffers) {
	glDeleteBuffers(n, buffers);
}

void gl::internal::buffer_data(unsigned target, unsigned size, const void* data, unsigned usage) {
	glBufferData(target, size, data, usage);
}

void gl::internal::buffer_sub_data(unsigned target, unsigned offset, unsigned size, const void* data) {
	glBufferSubData(target, offset, size, data);
}

void gl::internal::get_buffer_parameteriv(unsigned target, unsigned pname, int* data) {
	glGetBufferParameteriv(target, pname, data);
}