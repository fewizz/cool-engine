#pragma once
#include "with_name.hpp"
#include "primitive.hpp"
#include "bindable.hpp"
#include "vertex_array.hpp"
#include <string>

namespace gl {
	class shader;

	namespace internal {
		extern void bind_vertex_array(unsigned array);
		
		unsigned create_program();
		void delete_program(unsigned name);

		void attach_shader(unsigned program, unsigned shader);
		void detach_shader(unsigned program, unsigned shader);
		void link_program(unsigned program);
		void use_program(unsigned program);
		int get_uniform_location(unsigned program, const char *name);
		int get_attribute_location(unsigned program, const char *name);
		void draw_arrays(unsigned mode, int first, unsigned count);

		void uniform_1i(int location, int value);
		void uniform_1ui(int location, unsigned value);
		void uniform_4f(int location, float f1, float f2, float f3, float f4);
		void uniform_1iv(int location, unsigned count, const int* value);
		void uniform_matrix_4fv(int location, unsigned count, bool transpose, const float* value);
	}

	class program : public with_name {
	protected:
		static void draw_arrays_(primitive_type pt, unsigned start, size_t count);
	public:
		program():with_name(gl::internal::create_program()) {}
		program(program&& p):with_name{ std::move(p) } { }
		program(const program& p) = delete;

		~program() {
			if (name != invalid_name) {
				internal::delete_program(name);
				invalidate_name();
			}
		}

		template<class... Shaders>
		program(Shaders&... shs):program() {
			attach_(shs...);//  see below
			link();
			detach_(shs...);
		}
	public:
		template<class... Shaders>
		void attach(Shaders&... shaders) {
			attach_(shaders...);
		}

		template<class Shader>
		void attach(Shader& sh) {
			internal::attach_shader(name, ((with_name&)sh).name);
		}

		template<class... Shaders>
		void detach(Shaders&... shaders) {
			detach_(shaders...);
		}

		template<class Shader>
		void detach(Shader& sh) {
			internal::detach_shader(name, ((with_name&)sh).name);
		}
	private:
		template<class Shader, class... Shaders>
		void attach_(Shader& sh, Shaders&... shs) {
			attach(sh);
			attach_(shs...);
		}

		template<class Shader>
		void attach_(Shader& sh) {
			attach(sh);
		}

		template<class Shader, class... Shaders>
		void detach_(Shader& sh, Shaders&... shs) {
			detach(sh);
			detach_(shs...);
		}

		template<class Shader>
		void detach_(Shader& sh) {
			detach(sh);
		}

	public:
		void link() const {
			internal::link_program(name);
		}

		void use() const {
			internal::use_program(name);
		}

		void draw_arrays(primitive_type pt, unsigned start, unsigned count) const {
			use();
			internal::draw_arrays(pt, start, count);
		}

		void draw_arrays(primitive_type pt, unsigned start, unsigned count, vertex_array& vao) const {
			use();
			internal::bind_vertex_array(vao.name);
			internal::draw_arrays(pt, start, count);
		}

		unsigned attrib_location(std::string attrib_name) const {
			return internal::get_attribute_location(name, attrib_name.c_str());
		}

		unsigned uniform_location(std::string unifrom_name) const {
			return internal::get_uniform_location(name, unifrom_name.c_str());
		}

		void uniform(unsigned location, int value) const {
			use();
			internal::uniform_1i(location, value);
		}

		void uniform(unsigned location, unsigned value) const {
			use();
			internal::uniform_1ui(location, value);
		}

		void uniform(unsigned location, float v1, float v2, float v3, float v4) const {
			use();
			internal::uniform_4f(location, v1, v2, v3, v4);
		}

		template<class T, int N, class T0>
		void uniform(unsigned location, T0 value) const {
			static_assert(sizeof(T)*N == sizeof(T0));
			static_assert(N <= 4);

			T* data = (T*)&value;

			if constexpr (N == 1)uniform(location, data[0]);
			if constexpr (N == 2)uniform(location, data[0], data[1]);
			if constexpr (N == 3)uniform(location, data[0], data[2], data[3]);
			if constexpr (N == 4)uniform(location, data[0], data[1], data[2], data[3]);
		}

		// Second type
		template<class T, int N, class T2>
		void uniform(unsigned location, unsigned count, const T2* data) const {
			use();

			if constexpr(N == 1) {
				if constexpr (T == int)internal::uniform_1iv(location, count, data);
				else throw std::exception();
			}
			else throw std::exception();
		}

		// Third type
		template<class T, int C, int R, class T2>
		void uniform(unsigned location, unsigned count, bool transpose, const T2* value) const {
			static_assert(C*R * sizeof(T) == sizeof(T2));
			use();

			if constexpr (C == 4) {
				if constexpr (R == 4) {
					if constexpr (std::is_same_v<T, float>)internal::uniform_matrix_4fv(location, count, transpose, (T*)value);
					else throw std::exception();
				}
				else throw std::exception();
			}
			else throw std::exception();
		}

		template<class T, int C, int R, class T2>
		void uniform(unsigned location, T2& value) const {
			uniform<T, C, R, T2>(location, 1, false, &value);
		}

	};
}