#pragma once

#include <exception>
#include <string>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <map>
#include <array>
#include <memory>
#include <type_traits>

namespace gl {

	class context {
	public:
	};

	context wrap_context();

	enum internal_format:unsigned {
		rgba8 = 0x8058
	};

	enum pixel_format:unsigned {
		rgba = 0x1908
	};

	class texture : public bindable, with_name {
		static unsigned gen_();
		friend context;
	protected:
		enum texture_target :unsigned {
			texture_2d = 0x0DE1
		};
		texture_target target;
		static void bind(texture_target tar, unsigned name);

		texture(texture&& tex) :with_name{ std::move(tex) }, target{ tex.target }{}
		texture(texture_target tar) :with_name{ gen_() }, target{tar} {
			bind(tar, name);
		}

		int get_level_parameter_i(unsigned pname, int level = 0);
	public:
		~texture();
		void bind() override;

		unsigned width() {
			return get_level_parameter_i(0x1000);
		}
	};       

	class texture_2d : public texture {
		static void sub_image_2d(unsigned target, unsigned level, unsigned xo, unsigned yo, unsigned w, unsigned h, unsigned format, unsigned type, void* p);
		static void image_2d(unsigned target, unsigned if_, unsigned w, unsigned h, unsigned pf, unsigned type, void* data);

	public:
		texture_2d():texture{ texture_target::texture_2d } {}

		unsigned height() {
			return get_level_parameter_i(0x1001);
		}

		template<class T>
		void image(internal_format if_, unsigned w, unsigned h, pixel_format pf, T* data) {
			bind();
			image_2d(target, if_, w, h, pf, internal::type_token<T>(), data);
		}

		template<class Container>
		void image(internal_format if_, unsigned w, unsigned h, pixel_format pf, Container& data) {
			image(if_, w, h, pf, &*data.begin());
		}

		template<class T>
		void sub_image(int xo, int yo, int w, int h, pixel_format pf, T* data) {
			bind();
			sub_image_2d(target, 0, xo, yo, w, h, pf, internal::type_token<T>(), data);
		}

		void storage(unsigned levels, internal_format if_, unsigned w, unsigned h);
	};

	class texture_rect :public texture_2d {
	public:
	};

	enum wrap_mode :unsigned {
		clamp_to_border = 0x812D,
		clamp_to_edge = 0x812F,
		repeat = 0x2901
	};

	class sampler :public with_name {
		static unsigned gl_gen();

	public:
		sampler() :with_name{ gl_gen() } {}
		~sampler();
		void texture_wrap_s(wrap_mode wm);
		void texture_wrap_t(wrap_mode wm);
		void texture_wrap_r(wrap_mode wm);
	};

	class shader_compilation_error : public std::exception {
		friend shader;

		shader_compilation_error(shader*const s, std::string message) :shader_obj{ s }, std::exception(message.c_str()) {}
	public:
		shader * const shader_obj;
	};

	class shader:public with_name {
		friend program;
	protected:
		enum shader_type :unsigned {
			compute_shader = 0x91B9,
			vertex_shader = 0x8B31,
			fragment_shader = 0x8B30
		};
		shader_type type;

		static unsigned create(shader_type type);
		shader(shader_type type):with_name(create(type)), type {type} {}
		shader(shader_type type, std::string src):shader(type) { source(src); compile(); }
	public:
		shader(shader&) = delete;
		shader& operator=(shader&) = delete;
		shader(shader&& s) :with_name{ std::move(s) } { type = s.type; }
		~shader();

		void source(std::string src);
		void compile();
	};
	class vertex_shader : public shader {
	public:
		vertex_shader(std::string src) :shader(shader_type::vertex_shader, src) {}
	};
	class fragment_shader : public shader {
	public:
		fragment_shader(std::string src) :shader(shader_type::fragment_shader, src) {}
	};

	class program :public with_name {
	protected:
		static unsigned create();
		static void uniform_1iv(unsigned location, size_t count, const int* ptr);
		static void uniform_1uiv(unsigned location, size_t count, const unsigned* ptr);
		static void draw_arrays_(primitive_type pt, unsigned start, size_t count);
	public:
		~program();
		program():with_name(create()) {}
		program(program&& p):with_name{ std::move(p) } { }
		program(const program& p) = delete;

		template<class... Shaders>
		program(Shaders&... shs):program() {
			attach_(shs...);//  see below
			link();
			detach_(shs...);
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
		void attach(shader& sh);
		void detach(shader& sh);
		void link();
		void use();
		unsigned attrib_location(std::string attrib_name);
		unsigned unifrom_location(std::string unifrom_name);
		void uniform_matrix4fv(unsigned location, unsigned count, bool tr, float* values);
		void uniform_matrix4f(unsigned location, bool tr, float* values) {
			uniform_matrix4fv(location, 1, tr, values);
		}
		void uniform_matrix4f(unsigned location, float* values) {
			uniform_matrix4f(location, false, values);
		}
		void uniform_1ui(unsigned location, unsigned value);
		void uniform_1i(unsigned location, int value);
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
		
		void draw_arrays(primitive_type pt, unsigned start, size_t count) {
			use();
			draw_arrays(pt, start, count);
		}

		void draw_arrays(primitive_type pt, unsigned start, size_t count, vertex_array& vao) {
			use();
			vao.bind();
			draw_arrays_(pt, start, count);
		}
	};

	void active_texture(texture& tex, unsigned index);

	enum message_source :unsigned {
	};
	enum message_type :unsigned {
		error = 0x824C,
		deprecated_behavior,
		undefined_behavior,
		portability,
		performance,
		marker,
		push_group,
		pop_group,
		other
	};
	enum message_severity :unsigned {
	};

	void debug_message_callback(
		void(f)(message_source source, message_type type, unsigned id, message_severity severity, unsigned length,
			const char* message, const void* userParam));

	enum class clear_buffer: unsigned {
		color = 0x00004000,
		depth = 0x00000100
	};
	void clear_color(float r, float g, float b, float a);

	namespace internal {
		void clear(unsigned mask);
	}

	template<class... Mask>
	void clear(Mask... masks) {
		static_assert((std::is_same_v<gl::clear_buffer, Mask> && ...) );
		internal::clear((unsigned)( masks | ... ));
	}

	void viewport(int x, int y, unsigned w, unsigned h);

	void vertex_attrib2fv(unsigned index, const float* values);
	void vertex_attrib4fv(unsigned index, const float* values);
}