#include <iostream>
#include <memory>
#include "gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "lodepng.h"
#include <string>
#include <vector>
#include <initializer_list>

#define DEF_W 1000
#define DEF_H 600

using namespace gl;
using namespace std;

context* glc;

void debug(initializer_list<const string> lst) {
	for(auto s:lst)cout << s;
	cout << endl;
}
void debug(const string str) { debug({str}); }

int main() {
	if (!glfwInit()) debug("GLFW init error\n");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	GLFWwindow* window = glfwCreateWindow(DEF_W, DEF_H, "Test", NULL, NULL);

	glfwMakeContextCurrent(window);
	//GLenum err = glewInit();
	//debug({ "GLEW State: ", reinterpret_cast<const char*>(glewGetErrorString(err)) });
	glc = wrap_context();
	debug("GL context wrapped");
	debug_message_callback([](message_source, message_type, unsigned, message_severity, unsigned, const char* message, const void*) {
		debug({ "GL: ", message });
	});

	clear_color(0, 0, 0, 1);

	vector<uint8_t> bts;
	unsigned w, h;

	if (lodepng::decode(bts, w, h, "assets/bimo.png"))
		debug("error when reading texture");

	debug("Bimo png loaded");
	texture2d tex;
	tex.storage(1, internal_format::rgba8, w, h);
	tex.sub_image(0, 0, 0, w, h, pixel_format::rgba, bts);

	debug("Bimo loaded to va");

	program pr{ {
		vertex_shader,
			R"(
			#version 430 core
			in vec2 pos;
			out vec2 texc;
			void main() {
				texc = pos;
				gl_Position = vec4(pos, 0., 1.);
			}
			)"},{
		fragment_shader,
			R"(
			#version 430 core
			out vec4 color;
			in vec2 texc;
			layout(binding = 0) uniform sampler2D tex;
			void main() {
				color = texture(tex, vec2(texc.x, -texc.y));//vec4(1., 0., 0., 1.);
			}
			)"
	} };

	buffer verts(buffer_target::array_buffer);
	verts.data({
		0.F, 1.F,
		1.F, 0.F,
		1.F, 1.F,

		0.F, 0.F,
		1.F, 0.F,
		0.F, 1.F
		}, buffer_usage::static_draw);

	vertex_array vao;
	vao.vertex_attrib_pointer<float, 2>(verts, pr.attrib_location("pos"));
	vao.enable_vertex_attrib_array(pr.attrib_location("pos"));

	while (!glfwWindowShouldClose(window)) {
		clear({color_buffer});
		draw_arrays(primitive_type::triangles, 0, 6, pr, vao, { {0, tex} });
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}