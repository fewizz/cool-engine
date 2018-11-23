#pragma once
#include <string>
#include <vector>

namespace glfw {

	class window {
	public:
		class hint {
		protected:
			int code;
			int value;
		public:
			hint(int code, int value) :code{ code }, value{ value }{}
			void set_hint();
		};

		class bool_hint : public hint {
		public:
			bool_hint(unsigned code, bool value) :hint(code, value) {}
		};

		class opengl_debug_context : public bool_hint {
		public:
			opengl_debug_context(bool value) :bool_hint(0x00022007, value) {}
		};

		void* window_ptr;
		window(void* window_ptr) :window_ptr{ window_ptr } {}

		void make_context_current();
		bool should_close();
		void swap_buffers();
	};

	window&& create_window(int width, int height, std::string title, std::vector<window::hint> hints);
}