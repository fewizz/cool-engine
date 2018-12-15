#include "opengl/context.hpp"
#include "opengl/debug.hpp"
#include "opengl/core.hpp"
#include <iostream>
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
	glfw::window win = glfw::create_window(w, h, "Pro Player Simulator", { glfw::window::hints::opengl_debug_context{ true } });
	win.make_context_current();

	gl::context glc = wrap_context();
	debug_message_callback([](string message) { cout << message << "\n"; });

	win.swap_interval(1);

	freetype::library lib;

	freetype::face* face = lib.face_from_istream(ifstream{ "C:/Windows/Fonts/comic.ttf", std::ios::binary });
	face->set_char_size(64 * 30, 0, 0, 0);

	main_menu main(*face);

	clear_color(0, 0, 0, 1);

	while (!win.should_close())
	{
		std::tie(w, h) = win.get_framebuffer_size();

		viewport(0, 0, w, h);
		clear(clear_buffer::color);

		main.render();

		win.swap_buffers();
		glfw::poll_events();
	}
}