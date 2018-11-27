#pragma once

#include <exception>
#include <string>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <map>
#include <array>
#include <memory>

namespace gl {
	template<class T>
	std::shared_ptr<T> view(unsigned name) {
		T* ptr = new T(name);
		return std::shared_ptr<T>( ptr, [](T* p) {p->invalidate_name();  delete p; } );
	}

	template<class T>
	std::shared_ptr<T> view(T& t) {
		return view<T>(t.name);
	}

	namespace internal {
		template<class T>
		static unsigned constexpr gl_type_token() {
			if (std::is_same_v<T, int8_t>)
				return 0x1400;
			if (std::is_same_v<T, uint8_t>)
				return 0x1401;
			if (std::is_same_v<T, int32_t>)
				return 0x1404;
			if (std::is_same_v<T, uint32_t>)
				return 0x1405;
			if (std::is_same_v<T, float_t>)
				return 0x1406;
			else throw std::exception("GL: can't find appr. type");
		}
	}

	class with_name {
		template<class T>
		friend std::shared_ptr<T> view(T&);

		template<class T>
		friend std::shared_ptr<T> view(unsigned);
	protected:
		static constexpr unsigned invalid_name = std::numeric_limits<unsigned>::max();

		void invalidate_name() {
			name = invalid_name;
		}
	public:
		with_name(unsigned name):name{name}{}
		with_name(with_name&& o):name{ o.name } { 
			o.invalidate_name();
		}

		with_name& operator=(with_name&& o) {
			name = o.name;
			o.invalidate_name();
			return *this;
		}

		unsigned name{ invalid_name };
		//virtual ~with_name() = 0;
	};

	class bindable {
	protected:
		virtual void bind() = 0;
	};

	class vertex_array;
	class program;
	class shader;
	class texture;
	class texture_2d;

	class context {
	public:
	};

	context&& wrap_context();
	

	// §4.1
	class fence_sync;
	enum class sync_status:unsigned;

	class fence_sync {
		friend sync_status client_wait_sync(fence_sync, unsigned);
		friend void wait_sync(fence_sync);
		void* ptr;
	public:
		fence_sync();
	};

	enum class sync_status:unsigned {
		already_signaled = 0x911A,
		timeout_expired = 0x911B,
		condition_satisfied = 0x911C,
		wait_failed = 0x911D
	};

	sync_status client_wait_sync(fence_sync, unsigned);
	void wait_sync(fence_sync);

	// §4.2
	// TODO

	class query : public with_name {
		static unsigned gl_gen();
	};

	enum buffer_usage : unsigned {
		static_draw = 0x88E4
	};

	class buffer : public with_name, bindable {
		friend vertex_array;
		friend texture_2d;
	protected:
		enum buffer_target : unsigned {
			array = 0x8892,
			element_array = 0x8893,
			pixel_pack = 0x88EB,
			pixel_unpack = 0x88EC
		};

		static unsigned gl_gen();
		static void gl_bind(unsigned target, unsigned name);
		static void gl_parameteriv(unsigned tar, unsigned value, int* data);

		buffer(buffer_target tar) : with_name{ gl_gen() } {
			gl_bind(tar, name);
		}
		buffer(unsigned name) : with_name{ name } {}

		virtual buffer_target target() = 0;

		void bind() override { gl_bind(target(), name); }

		void data(size_t bytes, const void * data, buffer_usage usage = buffer_usage::static_draw);
	public:
		~buffer();
		template<class T>
		void data(T container, buffer_usage usage = buffer_usage::static_draw) {
			data(container.begin(), container.end(), usage);
		}

		size_t size() {
			bind();
			int size;
			gl_parameteriv(target(), 0x8764, &size);
			return size;
		}

		template<class RAI>
		void data(RAI begin, RAI end, gl::buffer_usage usage = gl::buffer_usage::static_draw) {
			data(std::distance(begin, end) * sizeof(std::iterator_traits<RAI>::value_type), &*begin, usage);
		}
	};

	class array_buffer : public buffer {
	public:
		array_buffer(unsigned name) : buffer(name) {} 
		array_buffer() : buffer(buffer_target::array) {}
		buffer_target target() override { return buffer_target::array; }
	};

	class element_array_buffer : public buffer {
	public:
		element_array_buffer(unsigned name) : buffer(name) {}
		element_array_buffer() : buffer(buffer_target::element_array) {}
		buffer_target target() override { return buffer_target::element_array; }
	};

	enum primitive_type :unsigned;
	void draw_arrays(primitive_type pt, unsigned start, unsigned count, std::shared_ptr<program> prog);

	namespace vertex_attribute {
		typedef unsigned int size;
		typedef unsigned int location;
		typedef bool normalized;
	}

	class vertex_array : public bindable, with_name {
		friend void draw_arrays(primitive_type, unsigned, unsigned, std::shared_ptr<program>);
		static unsigned gl_gen();
		static void gl_get_attribiv(unsigned index, unsigned pname, int* params);
		static void gl_vertex_attrib_pointer(unsigned index, unsigned size, unsigned type, bool normalized, unsigned stride, void* pointer);
		static void gl_vertex_attrib_i_pointer(unsigned index, unsigned size, unsigned type, unsigned stride, void* pointer);
		static void gl_bind(unsigned name);
	public:
		~vertex_array();

		vertex_array() : with_name{ gl_gen() } {
			bind();
		}
		vertex_array(vertex_array&& v):with_name{ std::move(v) } {}

		vertex_array(unsigned name):with_name{name} {}

		void bind() override { gl_bind(name); };

		std::shared_ptr<array_buffer> attrib_buffer(unsigned index) {
			bind();
			unsigned pointer[1];
			gl_get_attribiv(index, 0x889F, (int*)pointer);
			return view<array_buffer>(*pointer);
		}

		template<class T>
		void attrib_pointer(vertex_attribute::location location, vertex_attribute::size size, buffer& buff, vertex_attribute::normalized normalized = false) {
			bind();
			buff.bind();
			gl_vertex_attrib_pointer(location, size, internal::gl_type_token<T>(), normalized, 0, nullptr);
		}

		template<class T>
		void attrib_i_pointer(vertex_attribute::location location, vertex_attribute::size size, buffer& buff) {
			bind();
			buff.bind();
			gl_vertex_attrib_i_pointer(location, size, internal::gl_type_token<T>(), 0, nullptr);
		}

		unsigned attrib_size(unsigned index){
			bind();
			unsigned pointer[1];
			gl_get_attribiv(index, 0x8623, (int*)pointer);
			return *pointer;
		}

		void enable_attrib_array(unsigned index);
	};

	enum internal_format:unsigned {
		rgba8 = 0x8058
	};

	enum pixel_format:unsigned {
		rgba = 0x1908
	};

	class texture : public bindable, with_name {
		static unsigned gl_gen();
		friend context;
	protected:
		enum texture_target :unsigned {
			texture_2d = 0x0DE1
		};
		static void gl_bind(texture_target tar, unsigned name);

		virtual texture_target target() = 0;
		texture(texture_target tar) :with_name{ gl_gen() } {
			gl_bind(tar, name);
		}

	public:
		~texture();
		void bind() override;
	};       

	class texture_2d : public texture {
		static void gl_tex_sub_image_2d(unsigned target, unsigned level, unsigned xo, unsigned yo, unsigned w, unsigned h, unsigned format, unsigned type, void* p);
		static void gl_tex_image_2d(unsigned target, unsigned if_, unsigned w, unsigned h, unsigned pf, unsigned type, void* data);
	protected:
		texture_target target() override { return texture_target::texture_2d; }
	public:
		texture_2d():texture{ texture_target::texture_2d } {}

		template<class T>
		void image(internal_format if_, unsigned w, unsigned h, pixel_format pf, T* data) {
			bind();
			gl_tex_image_2d(target(), if_, w, h, pf, internal::gl_type_token<T>(), data);
		}

		template<class Container>
		void image(internal_format if_, unsigned w, unsigned h, pixel_format pf, Container& data) {
			image(if_, w, h, pf, &*data.begin());
		}

		void storage(unsigned levels, internal_format if_, unsigned w, unsigned h);
	};

	class texture_rect :public texture_2d {
	public:
		//texture_rect() :{texture_target::texture_2d} {}
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

		static unsigned gl_create(shader_type type);
		shader(shader_type type):with_name(gl_create(type)), type {type} {}
		shader(shader_type type, std::string src):shader(type) { source(src); compile(); }
	public:
		shader(shader&) = delete;
		shader& operator=(shader&) = delete;
		~shader();
		//virtual shader_type type() = 0;
		//shader(shader_type type, std::string src) :shader(type) { source(src); compile(); }

		void source(std::string src);
		void compile();
	};
	class vertex_shader : public shader {
		//shader_type type() override { return shader_type::vertex_shader; }
	public:
		vertex_shader(std::string src) :shader(shader_type::vertex_shader, src) {}
	};
	class fragment_shader : public shader {
		//shader_type type() override { return shader_type::fragment_shader; }
	public:
		fragment_shader(std::string src) :shader(shader_type::fragment_shader, src) {}
	};

	class program :public with_name {
	protected:
		static unsigned gl_create();
		static void gl_uniform_1iv(unsigned location, size_t count, const int* ptr);
		static void gl_uniform_1uiv(unsigned location, size_t count, const unsigned* ptr);
	public:
		~program();
		program():with_name(gl_create()) {}
		program(program&& p) :with_name{ std::move(p) } {}

		program(vertex_shader&& vs, fragment_shader&& fs,
			void(errorh)(shader_compilation_error error) = [](shader_compilation_error err) {}) : program()
		{
			try {
				attach(vs);
				attach(fs);
			}
			catch (shader_compilation_error e) { errorh(e); }
			link();
		}
		void attach(const shader& sh);
		void link();
		void use();
		unsigned attrib_location(std::string attrib_name);
		unsigned unifrom_location(std::string unifrom_name);
		void uniform_matrix4fv(unsigned location, unsigned count, bool tr, float* values);
		void uniform_1ui(unsigned location, unsigned value);
		void uniform_1i(unsigned location, int value);
		template<class RAI>
		void uniform_1uiv(unsigned location, RAI begin, RAI end) {
			use();
			gl_uniform_1uiv(location, std::distance(begin, end), &*(begin));
		}
		template<class Container>
		void uniform_1uiv(unsigned location, Container container) {
			uniform_1uiv(location, container.begin(), container.end());
		}
		template<class RAI>
		void uniform_1iv(unsigned location, RAI begin, RAI end) {
			use();
			gl_uniform_1iv(location, std::distance(begin, end), &*(begin));
		}
		template<class Container>
		void uniform_1iv(unsigned location, Container container) {
			uniform_1iv(location, container.begin(), container.end());
		}
		
	};

	/*class framebuffer : public bindable {
		static unsigned gl_gen();
		framebuffer():bindable(gl_gen()){
			bind();
		}
	};*/

	enum primitive_type :unsigned {
		lines = 1,
		line_loop,
		line_strip,
		triangles,
		triangle_strip,
		triangle_fan
	};
	void active_texture(texture& tex, unsigned index);

	void draw_arrays(primitive_type pt, unsigned start, unsigned count, std::shared_ptr<program> prog);
	void draw_arrays(primitive_type pt, unsigned start, unsigned count, std::shared_ptr<program> prog,
		std::shared_ptr<vertex_array> vao);
	void draw_arrays(primitive_type pt, unsigned start, unsigned count, std::shared_ptr<program> prog,
		std::shared_ptr<vertex_array> vao, std::map<const unsigned, texture*> texture_units);
	template<class Iterator>
	void draw_arrays(primitive_type pt, unsigned start, unsigned count, std::shared_ptr<program> prog,
		std::shared_ptr<vertex_array> vao, Iterator begin, Iterator end) {

		for (unsigned i = 0; begin != end; i++, begin++) 
			active_texture(**begin, i);
		draw_arrays(pt, start, count, prog, vao);
	}
	template<class Container>
	void draw_arrays(primitive_type pt, unsigned start, unsigned count, std::shared_ptr<program> prog,
		std::shared_ptr<vertex_array> vao, Container& container) {

		draw_arrays(pt, start, count, prog, vao, container.begin(), container.end());
	}

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

	enum clear_buffer: unsigned {
		color_buffer = 0x00004000,
		depth_buffer = 0x00000100
	};
	void clear_color(float r, float g, float b, float a);
	void clear(std::initializer_list<clear_buffer> mask);

	void viewport(int x, int y, unsigned w, unsigned h);

	void vertex_attrib2fv(unsigned index, const float* values);
	void vertex_attrib4fv(unsigned index, const float* values);
}