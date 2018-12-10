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

		static unsigned gen();
		static void bind(unsigned target, unsigned name);
		static void parameteriv(unsigned tar, unsigned value, int* data);

		buffer(buffer&& r) :with_name{ std::move(r) }, target{ r.target } { }
		buffer(internal::buffer_target tar) : with_name{ gen() }, target{ tar } {
			bind(tar, name);
		}
		buffer(unsigned name, internal::buffer_target tar) : with_name{ name }, target{ tar } {}

		void bind() override { bind(target, name); }

		void data(size_t bytes, const void * data, buffer_usage usage = buffer_usage::static_draw);
		void sub_data(unsigned offset, size_t bytes, const void * data);
	public:
		~buffer();

		size_t size() {
			bind();
			int size;
			parameteriv(target, 0x8764, &size);
			return size;
		}

		template<class RAI>
		void data(RAI begin, RAI end, gl::buffer_usage usage = gl::buffer_usage::static_draw) {
			data(std::distance(begin, end) * sizeof(std::iterator_traits<RAI>::value_type), &*begin, usage);
		}

		template<class Container>
		void data(Container container, buffer_usage usage = buffer_usage::static_draw) {
			data(sizeof(Container::value_type)*container.size(), container.data(), usage);
		}

		void data(size_t size, buffer_usage usage = buffer_usage::static_draw) {
			data(size, nullptr, usage);
		}

		template<class Container>
		void sub_data(unsigned offset, Container container) {
			sub_data(offset, sizeof(Container::value_type) * container.size(), container.data());
		}
	};
}