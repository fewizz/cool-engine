#include "texture.hpp"
#include "GL/glew.h"

void gl::internal::gen_textures(unsigned n, unsigned* textures) {
	glGenTextures(n, textures);
}

void gl::internal::bind_texture(unsigned target, unsigned texture) {
	glBindTexture(target, texture);
}

void gl::internal::delete_textures(unsigned n, unsigned* textures) {
	glDeleteTextures(n, textures);
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