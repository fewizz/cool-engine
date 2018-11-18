#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <iostream>
#include "opengl/gl.hpp"
#include "renderer.hpp"
#include <vector>
#include <initializer_list>
#include "freetype.hpp"
#include <fstream>
#include <filesystem>
#include "text_renderer.hpp"
#include <memory>
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "openal/al.hpp"
#include "wav.hpp"

using namespace gl;
using namespace std;
using namespace filesystem;

int main() {
	path p("C:\\Program Files (x86)\\OpenAL 1.1 SDK\\samples\\media\\stereo.wav");
	ifstream stream(p, std::ios::binary);
	wav::wav_audio_data_provider<ifstream> d{stream};
	d.next();
}

int main0() {
	al::device al_dev = al::open_device();
	al::context al_context = al_dev.create_context();

	path p("C:\\Windows\\Fonts\\comic.ttf");
	ifstream stream(p, std::ios::binary);
	size_t size = file_size(p);
	char* bytes = new char[size];
	stream.read(bytes, size);
	freetype::face face = freetype::load(bytes, bytes + size);
	//delete bytes;
	face.set_char_size(64*60, 64*60, 0, 0);
	//face.load_glyph(83);

	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	window = glfwCreateWindow(800, 600, "Hello", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gl::context* glc = wrap_context();
	debug_message_callback([](message_source source, message_type type, unsigned id, message_severity severity, unsigned length,
		const char* message, const void* userParam) {
		std::cout << message << "\n";
	});

	/*renderer_verticies triangle{ program{
		vertex_shader(R"(
#version 420 core

layout(location = 0) in vec2 pos;

void main() {
	gl_Position = vec4(pos, 0, 1);
}

)"),
		fragment_shader(R"(
#version 420 core

out vec4 color;

void main() {
	color = vec4(1, 1, 1, 1);
}
)")
		}, gl::primitive_type::triangles};

	triangle.vertex_attrib_data(vertex_attribute::location{ 0 }, vertex_attribute::size{ 2 }, vertex_attribute::normalized{ false },
		std::vector<float> {
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
		}
	);*/

	text_renderer tr{ u8"Так хмхмхм\nТы тута ?\nТобе куды песат ? ", face, make_shared<program>(
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

uniform sampler2D u_textures[16];

in vec2 uv_vs;
in flat int texture_unit_vs;
out vec4 color;

void main() {
	//color = texture(u_textures[texture_unit_vs], uv_vs + vec2(0.02, 0.02)) * vec4(1, 0, 0, 1);
	//color -= texture(u_textures[texture_unit_vs], uv_vs) * vec4(1, 0, 0, 1);
	//color += texture(u_textures[texture_unit_vs], uv_vs) * vec4(0, 1, 0, 1);

	color = texture(u_textures[texture_unit_vs], uv_vs);
}

)"}
	) };
	tr.matrix("u_mat", glm::translate(glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f), glm::vec3(-300.0, 200.0, 0.0)));

	while (!glfwWindowShouldClose(window))
	{
		clear_color(0, 0, 0, 1);
		clear({ color_buffer });

		tr.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}