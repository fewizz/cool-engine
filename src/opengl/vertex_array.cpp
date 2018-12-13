#include "vertex_array.hpp"

gl::vertex_array::~vertex_array() {
	if (name != invalid_name) {
		internal::delete_vertex_arrays(1, &name);
		invalidate_name();
	}
}

void gl::vertex_array::bind_vertex_buffer(unsigned binding_index, buffer& buffer) {
	bind();
	internal::bind_vertex_buffer(binding_index, ((with_name&)buffer).name, 0, 0);
}

void gl::vertex_array::enable_attrib_array(unsigned index) {
	bind();
	internal::enable_vertex_attrib_array(index);
}