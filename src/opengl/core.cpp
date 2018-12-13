#include "core.hpp"

void gl::clear_color(float r, float g, float b, float a) {
	internal::clear_color(r, g, b, a);
}

void gl::viewport(int x, int y, unsigned w, unsigned h) {
	internal::viewport(x, y, w, h);
}