#include "glfw/glfw.hpp"
#include "opengl/core.hpp"
#include "opengl/context.hpp"
#include "opengl/program.hpp"
#include "opengl/vertex_array.hpp"
#include "opengl/buffer.hpp"
#include "opengl/shader.hpp"
#include "renderer.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"

int w = 800;
int h = 600;

int main() {
	glfw::window window = glfw::create_window(w, h, "test", { glfw::window::hints::opengl_debug_context{true} });
	window.make_context_current();
	gl::context c{ gl::wrap_context() };
	//gl::internal::init();

	class rend : public gfx::verticies_renderer {
		gl::array_buffer vbo;

	public:
		rend() :gfx::verticies_renderer{ gl::program{
			gl::vertex_shader{ R"(
				#version 110
				uniform mat4 u_mat;
				attribute vec2 a_pos;

				void main() {
					gl_Position = vec4(a_pos, 0, 1) * u_mat;
				}
			)" },

			gl::fragment_shader{ R"(
				#version 110
				void main() {
					gl_FragColor = vec4(0, 0, 1, 1); // blue
				}
			)"}}} 
		{
			unsigned location = program->get_attrib_location("a_pos");
			vertex_array->attrib_pointer<float>(location , 2, vbo);
			vertex_array->enable_attrib_array(location);
			vbo.data(sizeof(float) * 3 * 2, gl::buffer_usage::dynamic_draw);
		}
	
		void render() override {
			vbo.sub_data(0, std::vector<float>{
				-w / 2.0f + 50, -h / 2.0f + 50,
				w / 2.0f - 50, -h / 2.0f + 50,
				-w / 2.0f + 50, h / 2.0f - 50
			});

			program->uniform_mat<4, 4, float>(program->get_unifrom_location("u_mat"), 1, false, &glm::ortho(-w / 2.0f, w / 2.0f, -h / 2.0f, h / 2.0f));
			program->draw_arrays(gl::primitive_type::triangles, 0, 3, *vertex_array);
		}
	} trinagle;

	while (!window.should_close()) {
		auto [w, h] = window.get_framebuffer_size();
		::w = w;
		::h = h;

		gl::viewport(0, 0, w, h);
		gl::clear(gl::clear_buffer::color);
		trinagle.render();
		window.swap_buffers();
		glfw::poll_events();
	}
}