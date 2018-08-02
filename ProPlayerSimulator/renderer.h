#pragma once
#include <vector>
#include <memory>

#include "gl.h"
#include "camera.h"

#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

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
class renderer_combined : public virtual renderer {
	std::vector<renderer*> vec;
public:
	renderer_combined() {}
	void render() override { for (auto r : vec) r->render(); }

	renderer_combined& operator+=(renderer r) {
		vec.push_back(&r);
	}
};
/////////////////////////////////////////
class quad_renderer :public virtual renderer {
	gl::vertex_array vao{};

public:
	//std::shared_ptr<gl::texture_2d> tex;//gl::texture_2d* tex_ptr;

	quad_renderer(glm::vec2 v0, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3) {
		glm::vec2 pos_arr[] = { v0, v1, v2, v3 };
		vao.vertex_attrib_pointer<float, 2>(0, pos_arr, 4*sizeof(glm::vec2));
		vao.enable_vertex_attrib_array(0);
	}

	void render() override { render(game::view * game::proj); }
	void render(glm::mat4 combined) {
		static gl::program program{
			gl::vertex_shader(R"(
#version 430 core

layout (location = 0) uniform mat4 mat;
layout (location = 0) in vec2 pos;

void main() {
	gl_Position = mat * vec4(pos, 0, 1);
}
			)"),
			gl::fragment_shader(R"(
#version 430 core

out vec4 color;
//layout (location = 4) uniform sampler2D tex;
//layout (location = 5) uniform uint flags;

void main() {
	color = vec4(1);//texture(tex, );
}
			)") };
		program.uniform_matrix4fv(0, 1, false, glm::value_ptr(combined));
		gl::draw_arrays(gl::primitive_type::triangles_fan, 0, 4, program, vao);
	}

};