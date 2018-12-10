#include "opengl/gl.hpp"
#include "gui.hpp"
#include "renderer.hpp"
#include <memory>
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "GLFW/glfw.hpp"

using namespace std;
using namespace gl;

int w = 800;
int h = 600;

int main() {
	glfw::window win = glfw::create_window(800, 600, "What", { glfw::window::hints::opengl_debug_context{ true } });
	win.make_context_current();

	gl::context glc = wrap_context();
	debug_message_callback([](message_source source, message_type type, unsigned id, message_severity severity, unsigned length,
		const char* message, const void* userParam) {
		cout << message << "\n";
	});

	shared_ptr<program> button_progam{ make_shared<program>(
		vertex_shader{ R"(
			#version 420 core
			in vec2 a_pos;
			uniform mat4 u_mat;

			void main() {
				gl_Position = u_mat * vec4(a_pos, 0, 1);
			}
			)" },
		fragment_shader{ R"(
			#version 420 core
			out vec4 color;

			void main() {
				color = vec4(0, 1, 0, 1);
			}
	)"} ) };

	class main_menu_view : public gui::view {
		class button : public gui::pressable, gui::view, gfx::verticies_renderer {
			array_buffer vbo;
			int x, y;

		public:
			button(int x, int y, float width, float height, shared_ptr<gl::program> program) :gfx::verticies_renderer(program), x{ x }, y{ y } {
				vbo.data(vector<float> {
					0.0f, 0.0f,
					width, 0.0f,
					width, height,
					0.0f, height
				});
			}

			void render() override {
				vertex_attribute::location pos_loc = program->attrib_location("a_pos");

				vertex_array->attrib_pointer<float>(pos_loc, 2, vbo);
				vertex_array->enable_attrib_array(pos_loc);
				program->uniform_matrix4fv(program->unifrom_location("u_mat"), 1, false, (float*)&glm::translate(glm::ortho(0.0f, (float)w, -h / 2.0f, h / 2.0f), {x, y, 0}));
				program->draw_arrays(triangle_strip, 0, 6, *vertex_array);
			}

			void update() override {
			}

			void press() override {
			}

			void unpress() override {
			}

		} start, options, quit;

	public:
		main_menu_view(std::shared_ptr<program> program) :
			start{ 100, 100, 300, 50, program },
			options{ 100, 0, 300, 50, program },
			quit{ 100, -100, 300, 50, program }
		{}

		void render() override {
			start.render();
			options.render();
			quit.render();
		}

		void update() override {

		}

	} main_menu(button_progam);

	clear_color(0, 0, 0, 1);

	while (!win.should_close())
	{
		auto [w, h] = win.get_framebuffer_size();
		::w = w;
		::h = h;

		viewport(0, 0, w, h);
		clear(clear_buffer::color);

		main_menu.render();

		win.swap_buffers();
		glfw::poll_events();
	}
}