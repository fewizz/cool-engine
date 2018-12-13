#include "shader.hpp"

void gl::shader::source(std::string src) {
	const char* c_str = src.c_str();
	internal::shader_source(name, 1, &c_str, nullptr);
}

void gl::shader::compile() {
	internal::compile_shader(name);
}