#include "program.hpp"
#include "GL/glew.h"

unsigned gl::internal::create_program() {
	return glCreateProgram();
}

void gl::internal::delete_program(unsigned program) {
	glDeleteProgram(program);
}

void gl::internal::attach_shader(unsigned program, unsigned shader) {
	glAttachShader(program, shader);
}

void gl::internal::detach_shader(unsigned program, unsigned shader) {
	glDetachShader(program, shader);
}

void gl::internal::link_program(unsigned program) {
	glLinkProgram(program);
}

void gl::internal::use_program(unsigned program) {
	glUseProgram(program);
}

int gl::internal::get_uniform_location(unsigned program, const char *name) {
	glGetUniformLocation(program, name);
}

int gl::internal::get_attribute_location(unsigned program, const char *name) {
	glGetAttribLocation(program, name);
}

void gl::internal::uniform_1i(int location, int value) {
	glUniform1i(location, value);
}

void gl::internal::uniform_1ui(int location, unsigned value) {
	glUniform1ui(location, value);
}