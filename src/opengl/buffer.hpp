#pragma once
#include "with_name.hpp"
#include "bindable.hpp"

namespace gl {
	namespace internal {
		enum buffer_target : unsigned {
			array = 0x8892,
			element_array = 0x8893,
			pixel_pack = 0x88EB,
			pixel_unpack = 0x88EC
		};

		void gen_buffers(unsigned n, unsigned* buffers);
		void bind_buffer(unsigned target, unsigned buffer);
		void delete_buffers(unsigned n, unsigned* buffers);

		void buffer_data(unsigned target, unsigned size, const void* data, unsigned usage);
		void buffer_sub_data(unsigned target, unsigned offset, unsigned size, const void* data);
		void get_buffer_parameteriv(unsigned target, unsigned pname, int* data);
	}

	enum buffer_usage : unsigned {
		static_draw = 0x88E4,
		dynamic_draw = 0x88E8
	};

	class vertex_array;
	class texture_2d;

	class buffer : public with_name, bindable {
		friend vertex_array;
		friend texture_2d;
	protected:
		internal::buffer_target target;

		buffer(buffer&& r) :with_name{ std::move(r) }, target{ r.target } { }

		buffer(internal::buffer_target tar) : target{ tar } {
			internal::gen_buffers(1, &name);
			internal::bind_buffer(tar, name);
		}

		buffer(unsigned name, internal::buffer_target tar) : with_name{ name }, target{ tar } {}

		void bind() override { internal::bind_buffer(target, name); }
	public:
		~buffer() {
			if (name != invalid_name) {
				internal::delete_buffers(1, &name);
				invalidate_name();
			}
		}

		size_t size() {
			bind();
			int size;
			gl::internal::get_buffer_parameteriv(target, 0x8764, &size);
			return size;
		}

		template<class RAI>
		void data(RAI begin, RAI end, gl::buffer_usage usage = gl::buffer_usage::static_draw) {
			bind();
			gl::internal::buffer_data(target, std::distance(begin, end) * sizeof(std::iterator_traits<RAI>::value_type), &*begin, usage);
		}

		template<class Container>
		void data(Container container, buffer_usage usage = buffer_usage::static_draw) {
			bind();
			gl::internal::buffer_data(sizeof(Container::value_type)*container.size(), container.data(), usage);
		}

		void data(size_t size, buffer_usage usage = buffer_usage::static_draw) {
			bind();
			gl::internal::buffer_data(target, size, nullptr, usage);
		}

		template<class Container>
		void sub_data(unsigned offset, Container container) {
			bind();
			internal::sub_data(target, offset, sizeof(Container::value_type) * container.size(), container.data());
		}
	};

	class array_buffer : public buffer {
	public:
		array_buffer(array_buffer&& r) :buffer(std::move(r)) {}

		template<class Container>
		array_buffer(Container c) : buffer(internal::buffer_target::array) {
			data<Container>(c);
		}
		array_buffer(unsigned name) : buffer(name, internal::buffer_target::array) {}
		array_buffer() : buffer(internal::buffer_target::array) {}
	};

	class element_array_buffer : public buffer {
	public:
		element_array_buffer(unsigned name) : buffer(name, internal::buffer_target::element_array) {}
		element_array_buffer() : buffer(internal::buffer_target::element_array) {}
	};
}