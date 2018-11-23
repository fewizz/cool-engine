#include "opengl/gl.hpp"
#include "GLFW/glfw3.h"
#include "gui.hpp"
#include "renderer.hpp"
#include <memory>
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

using namespace std;
using namespace gl;

int w = 800;
int h = 600;

int main() {
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	GLFWwindow* window = glfwCreateWindow(w, h, "Hello", NULL, NULL);

	glfwMakeContextCurrent(window);

	gl::context* glc = wrap_context();
	debug_message_callback([](message_source source, message_type type, unsigned id, message_severity severity, unsigned length,
		const char* message, const void* userParam) {
		std::cout << message << "\n";
	});

	class main_menu_view : public gui::view {
		class button : public gui::pressable, gui::view {
			verticies_renderer rend;
			int x, y = 0;
		public:
			button(int x, int y, float width, float height, std::shared_ptr<gl::program> program) :rend{ program, gl::primitive_type::triangles }, x{ x }, y{ y } {
				rend.vertex_attrib_data(program->attrib_location("a_pos"), 2, false, std::vector<float> {
					0.0f, 0.0f,
						width, 0.0f,
						width, height,
						0.0f, 0.0f,
						width, height,
						0.0f, height,
				});
				rend.matrix("u_mat", [&, x, y]() {
					return glm::translate(glm::ortho(0.0f, (float)::w, -(float)::h / 2.0f, (float)::h / 2.0f), { (float)x, (float)y, 0.0f });
				});
			}

			void render() override {
				rend.render();
			}

			void update() override {
			}

			void press() override {
			}

			void unpress() override {
			}

		} start, options, quit;

	public:
		main_menu_view(std::shared_ptr<gl::program> program) :
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

	} main_menu(make_shared<program>(
		vertex_shader{R"(
#version 420 core
in vec2 a_pos;
uniform mat4 u_mat;

void main() {
	gl_Position = u_mat * vec4(a_pos, 0, 1);
}

)"},
		fragment_shader{R"(
#version 420 core
out vec4 color;

void main() {
	color = vec4(0, 1, 0, 1);
}

)"}));

	while (!glfwWindowShouldClose(window))
	{
		viewport(0, 0, w, h);
		glfwGetFramebufferSize(window, &w, &h);
		clear_color(0, 0, 0, 1);
		clear({ color_buffer });

		main_menu.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}