#include "vertex_array.hpp"
#include "GL/glew.h"

void gl::internal::gen_vertex_arrays(unsigned n, unsigned* arrays) {
	glGenVertexArrays(n, arrays);
}

void gl::internal::bind_vertex_array(unsigned array) {
	glBindVertexArray(array);
}

void gl::internal::delete_vertex_arrays(unsigned n, unsigned* arrays) {
	glDeleteVertexArrays(1, arrays);
}

void gl::internal::vertex_attrib_pointer(unsigned index, int size, unsigned type, bool normalized, unsigned stride, const void *pointer) {
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void gl::internal::vertex_attrib_i_pointer(unsigned index, int size, unsigned type, unsigned stride, const void *pointer) {
	glVertexAttribIPointer(index, size, type, stride, pointer);
}

void gl::internal::bind_vertex_buffer(unsigned bindingindex, unsigned buffer, unsigned offset, unsigned stride) {
	glBindVertexBuffer(bindingindex, buffer, offset, stride);
}

void gl::internal::enable_vertex_attrib_array(unsigned index) {
	glEnableVertexAttribArray(index);
}

void gl::internal::get_vertex_attribiv(unsigned index, unsigned pname, int *params) {
	glGetVertexAttribiv(index, pname, params);
}