#include "main_menu.hpp"
#include <vector>
#include "freetype/freetype.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

using namespace std;
using namespace gl;
using namespace glm;

extern int w;
extern int h;

// main_menu_button
std::shared_ptr<main_menu_button_renderer> main_menu_button::renderer{};
std::shared_ptr<gl::program> main_menu_button::text_program{};
gl::vertex_attribute::location main_menu_button::pos_loc = -1;

void main_menu_button_renderer::render() {
	program->draw_arrays(gl::primitive_type::triangle_strip, 0, 4, *vertex_array);
}

main_menu_button::~main_menu_button() {
	if (renderer) {
		renderer.~shared_ptr();
		text_program.~shared_ptr();
		pos_loc = -1;
	}
}

main_menu_button::main_menu_button(float x, float y, float w, float h, freetype::face& face, string str)
	:x{ x }, y{ y }, w{ w }, h{ h }
{

	if (!renderer) {
		renderer.reset(new main_menu_button_renderer {
			program {
				vertex_shader {
					R"(
						#version 420 core
						uniform mat4 u_mat;
						in vec2 a_pos;
						void main() {
							gl_Position = u_mat * vec4(a_pos, 0, 1);
						}
					)"
				},
				fragment_shader {
					R"(
						#version 420 core
						out vec4 color;

						void main() {
							color = vec4(1, 0, 0, 1);
						}
					)"
				}
			}
		}
		);

		pos_loc = renderer->get_program()->attrib_location("a_pos");

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
						
				in vec2 uv_vs;
				out vec4 color;
						
				void main() {
					color = texture(u_atlas, uv_vs);
				}
			)"}
			}
		);
	}

	text_renderer.swap(make_shared<gfx::text_renderer>(str, face, text_program));

	vbo.data(vector<float>
	{
		0, 0,
		w, 0,
		0, h,
		w, h
	}
	);
}

void main_menu_button::render() {
	mat4 mat{ ortho(0.0f, (float)::w, -::h / 2.0f, ::h / 2.0f) };
	mat = translate(mat, { x, y, 0 });

	renderer->get_program()->uniform_matrix4f(renderer->get_program()->unifrom_location("u_mat"), (float*)&mat);
	renderer->get_vertex_array()->attrib_pointer<float>(pos_loc, 2, vbo);
	renderer->get_vertex_array()->enable_attrib_array(pos_loc);
	renderer->render();

	mat = translate(mat, { (w - text_renderer->get_width()) / 2, 10, 0 });
	text_renderer->get_program()->uniform_matrix4f(text_renderer->get_program()->unifrom_location("u_mat"), (float*)&mat);
	text_renderer->render();
}