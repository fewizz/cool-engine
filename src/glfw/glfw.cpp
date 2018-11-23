#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glfw/glfw.hpp"
#include <string>
#include "window.hpp"

void glfw::internal::init() {
	glfwInit();
}

void glfw::window::hint::set_hint() {
	glfwWindowHint(code, value);
}

glfw::window&& glfw::create_window(int width, int height, std::string title, std::vector<glfw::window::hint> hints) {
	internal::init();

	for (auto hint : hints)
		hint.set_hint();

	return glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

void glfw::window::make_context_current() {
	glfwMakeContextCurrent(static_cast<GLFWwindow*>(window_ptr));
}

bool glfw::window::should_close() {
	return glfwWindowShouldClose(static_cast<GLFWwindow*>(window_ptr));
}

void glfw::window::swap_buffers() {
	glfwSwapBuffers(static_cast<GLFWwindow*>(window_ptr));
}

void glfw::poll_events() {
	glfwPollEvents();
}
