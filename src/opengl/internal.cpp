#define GLEW_STATIC
#include "GL/glew.h"
#include "buffer.hpp"
#include "core.hpp"
#include "program.hpp"
#include "vertex_array.hpp"
#include "debug.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "context.hpp"

void gl::internal::init() {
	glewInit();
}

// Buffer
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

// Program
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
	return glGetUniformLocation(program, name);
}

int gl::internal::get_attribute_location(unsigned program, const char *name) {
	return glGetAttribLocation(program, name);
}

void gl::internal::draw_arrays(unsigned mode, int first, unsigned count) {
	glDrawArrays(mode, first, count);
}

void gl::internal::uniform_1i(int location, int value) {
	glUniform1i(location, value);
}

void gl::internal::uniform_1ui(int location, unsigned value) {
	glUniform1ui(location, value);
}

void gl::internal::uniform_4f(int location, float f1, float f2, float f3, float f4) {
	glUniform4f(location, f1, f2, f3, f4);
}

void gl::internal::uniform_1iv(int location, unsigned count, const int* value) {
	glUniform1iv(location, count, value);
}

void gl::internal::uniform_matrix_4fv(int location, unsigned count, bool transpose, const float* value) {
	glUniformMatrix4fv(location, count, transpose, value);
}

// Vertex Array
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

// Shader
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

// Core
void gl::internal::clear(unsigned mask) {
	glClear(mask);
}

void gl::internal::clear_color(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void gl::internal::viewport(int x, int y, unsigned w, unsigned h) {
	glViewport(x, y, w, h);
}

void gl::internal::enable(unsigned name) {
	glEnable(name);
}

void gl::internal::blend_func(unsigned source, unsigned destination) {
	glBlendFunc(source, destination);
}

// Texture
void gl::internal::gen_textures(unsigned n, unsigned* textures) {
	glGenTextures(n, textures);
}

void gl::internal::bind_texture(unsigned target, unsigned texture) {
	glBindTexture(target, texture);
}

void gl::internal::delete_textures(unsigned n, unsigned* textures) {
	glDeleteTextures(n, textures);
}

void gl::internal::texture_parameteri(unsigned texture, unsigned pname, int param) {
	glTextureParameteri(texture, pname, param);
}

void gl::internal::get_texture_level_parameteriv(unsigned texture, int level, unsigned pname, int* params) {
	glGetTextureLevelParameteriv(texture, level, pname, params);
}

void gl::internal::tex_image_2d(unsigned target, int level, int internalformat,
	unsigned width, unsigned height, int border, unsigned format, unsigned type, const void* data) {
	glTexImage2D(target, level, internalformat, width, height, border, format, type, data);
}

void gl::internal::tex_sub_image_2d(unsigned target, int level, int xoffset,
	int yoffset, unsigned width, unsigned height, unsigned format,
	unsigned type, const void *data) {
	glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, data);
}

void gl::internal::active_texture(unsigned texture) {
	glActiveTexture(texture);
}

// Debug
void gl::internal::debug_message_callback(debug_callback callback, const void *user_param) {
	glDebugMessageCallback((GLDEBUGPROC)callback, user_param);
}