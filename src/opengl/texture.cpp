#include "texture.hpp"

void gl::active_texture(texture& tex, unsigned index) {
	tex.bind();
	internal::active_texture(0x84C0 + index);
}