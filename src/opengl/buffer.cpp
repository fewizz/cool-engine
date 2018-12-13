#include "buffer.hpp"

gl::buffer::~buffer() {
	if (name != invalid_name) {
		gl::internal::delete_buffers(1, &name);
		invalidate_name();
	}
}