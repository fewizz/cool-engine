#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <initializer_list>

#include "gl.h"
#include "assets_loader.h"
#include "renderer.h"
#include "camera.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

	glc = wrap_context();
	debug("GL context wrapped");
	debug_message_callback([](message_source, message_type type, unsigned, message_severity, unsigned, const char* message, const void*) {
		debug({ "GL: ", message });
		if (type == message_type::error)
			//std::exit(0);
			throw std::exception("GL ERROR");
	});

	game::proj = glm::ortho(-DEF_W / 2.f, DEF_W / 2.f, -DEF_H / 2.f, DEF_H / 2.f, 0.f, 1.f);

	clear_color(0, 0, 0, 1);

	quad_renderer rend({ 0, 0 }, { 0, 100 }, { 100, 100 }, { 100, 0 });
	auto tex = game::assets_loader::load_texture2d("assets/bimo.png");
	rend.texture(&tex);

	while (!glfwWindowShouldClose(window)) {
		clear({color_buffer});
		rend.render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}