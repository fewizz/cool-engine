#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "text_renderer.hpp"
#include <fstream>
#include <filesystem>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glfw/glfw.hpp"

using namespace std;
using namespace gl;
using namespace filesystem;
using namespace glfw;

int main() {
	path path("C:\\Windows\\Fonts\\comic.ttf");
	ifstream stream(path, std::ios::binary);

	vector<char> bytes(file_size(path));
	stream.read(bytes.data(), bytes.size());
	freetype::face face{ freetype::load(bytes) };
	face.set_char_size(64 * 60, 0, 0, 0);

	//glfwInit();
	glfw::window window{ glfw::create_window(800, 600, "Test", {window::opengl_debug_context(true)}) };

	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	//GLFWwindow* window = glfwCreateWindow(800, 600, "Hello", NULL, NULL);

	//glfwMakeContextCurrent(window);
	window.make_context_current();

	gl::context* glc = wrap_context();
	debug_message_callback([](message_source source, message_type type, unsigned id, message_severity severity, unsigned length,
		const char* message, const void* userParam) {
		std::cout << message << "\n";
	});

	text_renderer tr{ "The quick brown fox\njumps over the lazy dog", face, make_shared<program>(
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

	while (!window.should_close())
	{
		clear_color(0, 0, 0, 1);
		clear({ color_buffer });

		tr.render();

		window.swap_buffers();
		poll_events();
	}

	glfwTerminate();
}