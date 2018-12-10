#include "opengl/gl.hpp"
#include "gui.hpp"
#include "renderer.hpp"
#include <memory>
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "GLFW/glfw.hpp"
#include "main_menu.hpp"
#include <fstream>

using namespace std;
using namespace gl;

int w = 800;
int h = 600;

int main() {
	glfw::window win = glfw::create_window(800, 600, "Pro Player Simulator", { glfw::window::hints::opengl_debug_context{ true } });
	win.make_context_current();

	gl::context glc = wrap_context();
	debug_message_callback([](message_source source, message_type type, unsigned id, message_severity severity, unsigned length,
		const char* message, const void* userParam) {
		cout << message << "\n";
	});

	freetype::library lib;

	freetype::face* face = lib.face_from_istream(ifstream{ "C:/Windows/Fonts/comic.ttf", std::ios::binary });
	face->set_char_size(64 * 30, 0, 0, 0);

	main_menu main(*face);

	clear_color(0, 0, 0, 1);

	while (!win.should_close())
	{
		auto [w, h] = win.get_framebuffer_size();
		::w = w;
		::h = h;

		viewport(0, 0, w, h);
		clear(clear_buffer::color);

		main.render();

		win.swap_buffers();
		glfw::poll_events();
	}
}