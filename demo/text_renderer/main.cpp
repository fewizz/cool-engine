#include "text_renderer.hpp"
#include <fstream>
#include <filesystem>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glfw/glfw.hpp"
#include "freetype/library.hpp"

using namespace std;
using namespace gl;
using namespace filesystem;
using namespace glfw;

int main() {
	freetype::library lib;

	freetype::face* face = lib.face_from_istream(ifstream{ "C:/Windows/Fonts/comic.ttf", std::ios::binary });
	face->set_char_size(64 * 60, 0, 0, 0);

	glfw::window window{ glfw::create_window(800, 600, "Test", {window::opengl_debug_context(true)}) };

	window.make_context_current();

	gl::context glc{wrap_context()};
	debug_message_callback([](message_source source, message_type type, unsigned id, message_severity severity, unsigned length,
		const char* message, const void* userParam) {
		std::cout << message << "\n";
	});

	gfx::text_renderer tr{ u8"The quick brown fox\njumps over the lazy dog", *face, make_shared<program>(
		vertex_shader{R"(
#version 420 core

uniform mat4 u_mat;

in vec2 a_position;
in vec2 a_uv;
in int a_texture_unit;

out vec2 uv_vs;
out flat int texture_unit_vs;

void main() {
	uv_vs = a_uv;
	texture_unit_vs = a_texture_unit;
	gl_Position = u_mat * vec4(a_position, 0, 1);
}
)"},
		fragment_shader{R"(
#version 420 core

uniform sampler2D u_textures[32];

in vec2 uv_vs;
in flat int texture_unit_vs;
out vec4 color;

void main() {
	color = texture(u_textures[texture_unit_vs], uv_vs);
}
)"}
		)
	};

	tr.matrix("u_mat", []() {
		return glm::translate(glm::ortho(-400.0, 400.0, -300.0, 300.0), {-400.0, 200.0, 0});
	});

	gl::clear_color(0, 0, 0, 1);
	while (!window.should_close())
	{
		clear({ color_buffer });

		tr.render();

		window.swap_buffers();
		poll_events();
	}
}