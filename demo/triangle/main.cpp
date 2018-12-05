#include "glfw/glfw.hpp"
#include "opengl/gl.hpp"
#include "renderer.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"

int w = 800;
int h = 600;

int main() {
	glfw::window window = glfw::create_window(w, h, "test", { glfw::window::hints::opengl_debug_context{true} });
	window.make_context_current();
	gl::context c{ gl::wrap_context() };

	class rend : public gfx::renderer {
		gl::program prog;
		gl::vertex_array vao;
		gl::array_buffer vbo;

	public:
		rend():gfx::renderer() {
			prog.attach(gl::vertex_shader{ R"(
				#version 110
				uniform mat4 u_mat;
				attribute vec2 a_pos;

				void main() {
					gl_Position = vec4(a_pos, 0, 1) * u_mat;
				}
			)" });

			prog.attach(gl::fragment_shader{ R"(
				#version 110
				void main() {
					gl_FragColor = vec4(0, 0, 1, 1); // blue
				}
			)"});

			prog.link();

			unsigned location = prog.attrib_location("a_pos");
			vao.attrib_pointer<float>(location , 2, vbo);
			vao.enable_attrib_array(location);
		}
	
		void render() override {
			vbo.data(std::vector<float>{
				-w / 2.0f + 50, -h / 2.0f + 50,
				w / 2.0f - 50, -h / 2.0f + 50,
				-w / 2.0f + 50, h / 2.0f - 50
			});

			prog.uniform_matrix4fv(prog.unifrom_location("u_mat"), 1, false, (float*)&glm::ortho(-w / 2.0f, w / 2.0f, -h / 2.0f, h / 2.0f));
			gl::draw_arrays(gl::primitive_type::triangles, 0, 3, prog, vao);
		}
	} trinagle;

	while (!window.should_close()) {
		auto [w, h] = window.get_framebuffer_size();
		::w = w;
		::h = h;

		gl::viewport(0, 0, w, h);
		gl::clear(gl::clear_buffer::color_buffer);
		trinagle.render();
		window.swap_buffers();
		glfw::poll_events();
	}
}