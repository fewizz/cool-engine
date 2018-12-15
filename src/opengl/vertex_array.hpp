#pragma once
#include "bindable.hpp"
#include "with_name.hpp"
#include "buffer.hpp"
#include "internal.hpp"
#include "primitive.hpp"

namespace gl {
	namespace internal {
		void gen_vertex_arrays(unsigned n, unsigned* arrays);
		void bind_vertex_array(unsigned array);
		void delete_vertex_arrays(unsigned n, unsigned* arrays);

		void vertex_attrib_pointer(unsigned index, int size, unsigned type, bool normalized, unsigned stride, const void *pointer);
		void vertex_attrib_i_pointer(unsigned index, int size, unsigned type, unsigned stride, const void *pointer);
		void bind_vertex_buffer(unsigned bindingindex, unsigned buffer, unsigned offset, unsigned stride);
		void enable_vertex_attrib_array(unsigned index);
		void get_vertex_attribiv(unsigned index, unsigned pname, int *params);
	}

	namespace vertex_attribute {
		typedef unsigned int size;
		typedef unsigned int location;
		typedef bool normalized;
	}

	class program;

	class vertex_array : public bindable, with_name {
		friend class program;

		void bind() override { internal::bind_vertex_array(name); };
	public:
		~vertex_array() {
			if (name != invalid_name) {
				internal::delete_vertex_arrays(1, &name);
				invalidate_name();
			}
		}

		vertex_array() {
			internal::gen_vertex_arrays(1, &name);
			bind();
		}

		vertex_array(vertex_array&& v) :with_name{ std::move(v) } {}

		vertex_array(unsigned name) :with_name{ name } {}

		std::shared_ptr<array_buffer> attrib_array_buffer(unsigned index) {
			bind();
			unsigned buffer;
			internal::get_vertex_attribiv(index, 0x889F, (int*)&buffer);
			return internal::view<array_buffer>(buffer);
		}

		template<class T, int size>
		void attrib_pointer(vertex_attribute::location location, array_buffer& buff, vertex_attribute::normalized normalized = false) {
			bind();
			buff.bind();
			internal::vertex_attrib_pointer(location, size, internal::type_token<T>(), normalized, 0, nullptr);
		}

		template<class T, int size>
		void attrib_i_pointer(vertex_attribute::location location, array_buffer& buff) {
			bind();
			buff.bind();
			internal::vertex_attrib_i_pointer(location, size, internal::type_token<T>(), 0, nullptr);
		}

		unsigned attrib_size(unsigned index) {
			bind();
			unsigned size;
			internal::get_vertex_attribiv(index, 0x8623, (int*)&size);
			return size;
		}

		void bind_vertex_buffer(unsigned binding_index, buffer& buffer) {
			bind();
			internal::bind_vertex_buffer(binding_index, ((with_name&)buffer).name, 0, 0);
		}

		void enable_attrib_array(unsigned index) {
			bind();
			internal::enable_vertex_attrib_array(index);
		}
	};
}