#pragma once
#include "with_name.hpp"
#include "primitive.hpp"
#include "bindable.hpp"
#include <string>

namespace gl {
	class shader;
	class vertex_array;

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
		void link() {
			internal::link_program(name);
		}

		void use() {
			internal::use_program(name);
		}

		void draw_arrays(primitive_type pt, unsigned start, unsigned count) {
			use();
			internal::draw_arrays(pt, start, count);
		}

		void draw_arrays(primitive_type pt, unsigned start, unsigned count, vertex_array& vao) {
			use();
			internal::bind_vertex_array(((with_name&)vao).name);
			internal::draw_arrays(pt, start, count);
		}

		unsigned get_attrib_location(std::string attrib_name) {
			return internal::get_attribute_location(name, attrib_name.c_str());
		}

		unsigned get_unifrom_location(std::string unifrom_name) {
			return internal::get_uniform_location(name, unifrom_name.c_str());
		}
		
		void uniform(unsigned location, int value) {
			use();
			internal::uniform_1i(location, value);
		}

		void uniform(unsigned location, unsigned value) {
			use();
			internal::uniform_1ui(location, value);
		}

		template<class RAI>
		void uniform_1uiv(unsigned location, RAI begin, RAI end) {
			use();
			uniform_1uiv(location, std::distance(begin, end), &*(begin));
		}

		template<class Container>
		void uniform_1uiv(unsigned location, Container container) {
			uniform_1uiv(location, container.begin(), container.end());
		}

		template<class RAI>
		void uniform_1iv(unsigned location, RAI begin, RAI end) {
			use();
			uniform_1iv(location, std::distance(begin, end), &*(begin));
		}

		template<class Container>
		void uniform_1iv(unsigned location, Container container) {
			uniform_1iv(location, container.begin(), container.end());
		}

		template<int C, int R, class T>
		void uniform_mat(unsigned location, unsigned count, bool transpose, const void* values);

		template<>
		void uniform_mat<4, 4, float>(unsigned location, unsigned count, bool transpose, const void* values) {
			use();
			internal::uniform_matrix_4fv(location, count, transpose, (float*)values);
		}


	};
}