#include "main_menu.hpp"
#include <vector>
#include "freetype/freetype.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "opengl/shader.hpp"

using namespace std;
using namespace gl;
using namespace glm;

extern int w;
extern int h;

// main_menu_button
std::shared_ptr<gl::program> main_menu_button::rectangle_program{};
std::shared_ptr<gl::program> main_menu_button::text_program{};

main_menu_button::~main_menu_button() {
	if (rectangle_program) {
		rectangle_program.~shared_ptr();
		text_program.~shared_ptr();
	}
}

main_menu_button::main_menu_button(float x, float y, float w, float h, freetype::face& face, string str)
	:x{ x }, y{ y }, w{ w }, h{ h }
{

	if (!rectangle_program) {
		rectangle_program.reset(new program {
			vertex_shader {R"(
				#version 420 core
				uniform mat4 u_mat;
				in vec2 a_position;
				void main() {
					gl_Position = u_mat * vec4(a_position, 0, 1);
				}
			)"},
			fragment_shader {R"(
				#version 420 core
				out vec4 color;

				void main() {
					color = vec4(1, gl_FragCoord.x / 500.0, 0, 0.4);
				}
			)"}
		});

		text_program.reset(new program {
			vertex_shader{ R"(
				#version 420 core

				uniform mat4 u_mat;

				in vec2 a_position;
				in vec2 a_uv;
						
				out vec2 uv_vs;
						
				void main() {
					uv_vs = a_uv;
					gl_Position = u_mat * vec4(a_position, 0, 1);
				}
			)" },
			fragment_shader { R"(
				#version 420 core
						
				uniform sampler2D u_atlas;
				uniform vec4 u_color;
						
				in vec2 uv_vs;
				out vec4 color;
						
				void main() {
					color = texture(u_atlas, uv_vs) * u_color;
				}
			)"}
			}
		);
	}

	rectangle_renderer.swap(make_shared<gfx::rectangle_renderer>(x, y, w, h, rectangle_program));
	text_renderer.swap(make_shared<gfx::text_renderer>(str, face, text_program));
}

void main_menu_button::render() {
	mat4 mat{ ortho(0.0f, (float)::w, -::h / 2.0f, ::h / 2.0f) };
	rectangle_program->uniform<float, 4, 4>(rectangle_program->uniform_location("u_mat"), mat);
	rectangle_renderer->render();

	mat = translate(mat, { x + (w - text_renderer->get_width()) / 2, y + 15, 0 });
	text_program->uniform<float, 4, 4>(text_program->uniform_location("u_mat"), mat);
	text_program->uniform<float, 4>(text_program->uniform_location("u_color"), vec4{ 0, 0, 0, 1 });
	text_renderer->render();

	mat = translate(mat, { 2, 2, 0 });
	text_program->uniform<float, 4, 4>(text_program->uniform_location("u_mat"), mat);
	text_program->uniform<float, 4>(text_program->uniform_location("u_color"), vec4{ 1, 1, 1, 1 });
	text_renderer->render();
}