#include "shader.hpp"
#include "GL/glew.h"

unsigned gl::internal::create_shader(unsigned type) {
	return glCreateShader(type);
}

void gl::internal::delete_shader(unsigned shader) {
	glDeleteShader(shader);
}

void gl::internal::shader_source(unsigned shader, unsigned count, const char* const* string, const int* length) {
	glShaderSource(shader, count, string, length);
}

void gl::internal::compile_shader(unsigned shader) {
	glCompileShader(shader);
}