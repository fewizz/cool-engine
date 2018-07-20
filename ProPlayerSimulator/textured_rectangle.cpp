#include "textured_rectangle.h"
#include "gl.h"

static gl::program *p;

void textured_rectangle::render() {
	if (p == nullptr) {
		p = new gl::program{ 
			{gl::shader_type::vertex_shader, 
			R"(
#version 430 core
			
layout (location = 0) attrib vec2 pos;
layout (location = 1) attrib vec2 dim;

void main() {
	gl_Position = 
}
			)"
			} };
	}
}