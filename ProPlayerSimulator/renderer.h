#pragma once
#include "gl.h"
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include <vector>

//class renderer_combined;
class matrix_provider {
	virtual glm::mat4x4 combined() = 0;
	glm::mat4x4 operator()() { return combined(); }
};

class renderer {
public:
	renderer() = default;
	renderer(const renderer&) = delete;
	renderer operator=(const renderer&) = delete;
	virtual void render() {}
};
//////////////////////////////////////////

/*class renderer_combinable:public renderer {
public:
	virtual bool combinable(renderer other) { return false; };
};*/
//////////////////////////////////////////
class renderer_combined : public virtual renderer {
	std::vector<renderer&> vec;
public:
	renderer_combined() {}
	void render() override { for (auto &&r : vec) r.render(); }

	/*renderer_combined operator+(const renderer & other) {
		renderer_combined o{ *this };
		o.vec.push_back(other);
		return o;
	}*/

	renderer_combined& operator+=(renderer r) {
		vec.push_back(r);
	}

	/*renderer_combined& operator+=(renderer_combined r) {
		for (auto r0 : r.vec) vec.push_back(r0);
		return *this;
	}*/
};
/////////////////////////////////////////
/*class renderer_with_program: public virtual renderer {
	const gl::program& p;
public:
	renderer_with_program(const gl::program& pr) :p{ pr } {}
};*/
/////////////////////////////////////////
class quad_renderer :public virtual renderer {
public:
	quad_renderer(matrix_provider mp, glm::vec2 p1, glm::vec2 p2) {

	}

	void render() override {
		static gl::program program{
			gl::vertex_shader(R"(
#version 430 core

layout (location = 0) uniform mat4;
layout (location = 1) uniform vec2 v[4];

void main() {
	gl_Position = mat * vec4(v[gl_VertexID]);
}
			)"),
			gl::fragment_shader(R"(
#version 430 core

out vec4 c;
void main() {
	c = vec4(1.);
}
			)")
		};


	}

};