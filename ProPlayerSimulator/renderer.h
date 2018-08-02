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
	gl::texture_2d* tex;

	quad_renderer(
		glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3,
		glm::vec2 t0, glm::vec2 t1, glm::vec2 t2, glm::vec2 t3) 
	{
		glm::vec2 pos_arr[] = { p0, p1, p2, p3 };
		glm::vec2 tex_arr[] = { t0, t1, t2, t3 };
		vao.attrib_pointer<float, 2>(0, pos_arr, 4*sizeof(glm::vec2));
		vao.enable_attrib_array(0);

		vao.attrib_pointer<float, 2>(1, tex_arr, 4 * sizeof(glm::vec2));
		vao.enable_attrib_array(1);
	}

	quad_renderer(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
		:quad_renderer {
			p0, p1, p2, p3, {0.f, 1.f}, {0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}
		} {}

	void texture(gl::texture_2d* tex) { this->tex = tex; }

	void render() override { render(game::view * game::proj); }
	void render(glm::mat4 combined) {
		static gl::program program{
			gl::vertex_shader(R"(
#version 430 core

layout (location = 0) uniform mat4 mat;
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
out vec2 vs_tex;

void main() {
	gl_Position = mat * vec4(pos, 0, 1);
	vs_tex = tex;
}
			)"),
			gl::fragment_shader(R"(
#version 430 core

in vec2 vs_tex;
out vec4 color;
layout (binding = 0) uniform sampler2D tex;
//layout (location = 5) uniform uint flags;

void main() {
	color = texture(tex, vs_tex);
}
			)") };
		gl::active_texture(*tex, 0);
		program.uniform_matrix4fv(0, 1, false, glm::value_ptr(combined));
		gl::draw_arrays(gl::primitive_type::triangles_fan, 0, 4, program, vao);
	}

};