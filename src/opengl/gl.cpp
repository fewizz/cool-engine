#define GLEW_STATIC
#include "GL/glew.h"
#include "opengl/gl.hpp"
#include <exception>
#include <iterator>

namespace gl {
	/*template<class T>
	object_view<T> wrap_globject(unsigned name) {
		T* o = (T*)(&name);
		return object_view(std::move(*o));
	}*/

	context* wrap_context() {
		glewInit();
		return new context{};
	}
	// fence_sync
	fence_sync::fence_sync() :ptr{ glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0) } {}
	sync_status client_wait_sync(fence_sync s, unsigned timeout) { 
		return static_cast<sync_status>(glClientWaitSync(reinterpret_cast<GLsync>(s.ptr), GL_SYNC_FLUSH_COMMANDS_BIT, timeout)); 
	}
	void wait_sync(fence_sync s) { glWaitSync(reinterpret_cast<GLsync>(s.ptr), 0, GL_TIMEOUT_IGNORED); }

	// query
	unsigned query::gl_gen() { return 0; };

	// buffer
	unsigned buffer::gl_gen() { unsigned n; glGenBuffers(1, &n); return n; }
	void buffer::gl_bind(unsigned t, unsigned n) {
		glBindBuffer(t, n);
	}
	void buffer::del() { glDeleteBuffers(1, &name); invalidate_name(); }

	void buffer::gl_parameteriv(unsigned tar, unsigned value, int* data) {
		glGetBufferParameteriv(tar, value, data);
	}

	void buffer::data(size_t bytes, const void * data, buffer_usage usage) {
		bind();
		glBufferData(target(), bytes, data, usage);
	}

	// vertex_array
	unsigned vertex_array::gl_gen() {
		unsigned name;
		glGenVertexArrays(1, &name);
		return name;
	}
	void vertex_array::gl_bind(unsigned name) { glBindVertexArray(name); }
	void vertex_array::gl_get_attribiv(unsigned index, unsigned pname, int* params) {
		glGetVertexAttribiv(index, pname, params);
	}
	void vertex_array::del() { glDeleteBuffers(1, &name); invalidate_name(); }

	void vertex_array::gl_vertex_attrib_pointer(unsigned index, unsigned size, unsigned type, bool normalized, unsigned stride, void* pointer) {
		glVertexAttribPointer(index, size, type, normalized, stride, nullptr);
	}

	void vertex_array::gl_vertex_attrib_i_pointer(unsigned index, unsigned size, unsigned type, unsigned stride, void* pointer) {
		glVertexAttribIPointer(index, size, type, stride, nullptr);
	}

	void vertex_array::enable_attrib_array(unsigned index) {
		bind();
		glEnableVertexAttribArray(index);
	}

	// texture
	unsigned texture::gl_gen() {
		unsigned name;
		glGenTextures(1, &name);
		return name;
	}
	void texture::gl_bind(texture_target tar, unsigned name) {
		glBindTexture(tar, name);
	}
	void texture::bind() { gl_bind(target(), name); }
	void texture::del() { glDeleteBuffers(1, &name); invalidate_name(); }

	// texture2d
	void texture_2d::gl_tex_sub_image_2d(unsigned target, unsigned level, unsigned xo, unsigned yo, unsigned w, unsigned h, unsigned format, unsigned type, void* p) {
		glTexSubImage2D(target, level, xo, yo, w, h, format, type, p);
	}

	void texture_2d::gl_tex_image_2d(unsigned target, unsigned if_, unsigned w, unsigned h, unsigned pf, unsigned type, void* data) {
		glTexImage2D(target, 0, if_, w, h, 0, pf, type, data);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}

	void texture_2d::storage(unsigned levels, internal_format if_, unsigned w, unsigned h) {
		bind();
		glTexStorage2D(target(), levels, if_, w, h);
	}

	// sampler
	unsigned sampler::gl_gen() { 
		unsigned name;
		glGenSamplers(1, &name);
		return name;
	}
	void sampler::del() { glDeleteSamplers(1, &name); }

	void sampler::texture_wrap_s(wrap_mode wm) { glSamplerParameteri(name, GL_TEXTURE_WRAP_S, wm); }
	void sampler::texture_wrap_t(wrap_mode wm) { glSamplerParameteri(name, GL_TEXTURE_WRAP_T, wm); }
	void sampler::texture_wrap_r(wrap_mode wm) { glSamplerParameteri(name, GL_TEXTURE_WRAP_R, wm); }
	// shader
	unsigned shader::gl_create(shader::shader_type type) { return glCreateShader(type); }
	void shader::del() { glDeleteShader(name); invalidate_name(); }

	void shader::source(std::string src) {
		const char* s { src.c_str() };
		glShaderSource(name, 1, &s, nullptr);
	}

	void shader::compile() {
		glCompileShader(name);
		GLint i{};
		GLint* status{ &i };
		glGetShaderiv(name, GL_COMPILE_STATUS, status);
		if (*status == GL_FALSE) {
			glGetShaderiv(name, GL_INFO_LOG_LENGTH, status);
			char* mess = new char[*status];
			glGetShaderInfoLog(name, *status, nullptr, mess);
			throw shader_compilation_error(this, mess);
		}
	}

	// program
	unsigned program::gl_create() { return glCreateProgram(); }
	void program::del() { glDeleteProgram(name); invalidate_name(); }

	void program::attach(const shader& sh) { glAttachShader(name, sh.name); }
	void program::link() { glLinkProgram(name); }
	void program::use() { glUseProgram(name); }
	unsigned program::attrib_location(std::string attrib_name) {
		int loc = glGetAttribLocation(name, attrib_name.c_str());
		if (loc == -1) throw std::exception("location not found");
		return loc;
	};
	unsigned program::unifrom_location(std::string unifrom_name) {
		int loc = glGetUniformLocation(name, unifrom_name.c_str());
		if (loc == -1) throw std::exception("location not found");
		return loc;
	}
	void program::uniform_matrix4fv(unsigned location, unsigned count, bool tr, float * values) {
		use();
		glUniformMatrix4fv(location, count, tr, values);
	}

	void program::uniform_1ui(unsigned location, unsigned value) { use(); glUniform1ui(location, value); }
	void program::uniform_1i(unsigned location, int value) { use(); glUniform1i(location, value); }
	void program::gl_uniform_1iv(unsigned location, size_t count, const int* ptr) { glUniform1iv(location, count, ptr); }
	void program::gl_uniform_1uiv(unsigned location, size_t count, const unsigned* ptr) { glUniform1uiv(location, count, ptr); }
	//void program::uniform_1uiv(unsigned location, unsigned* begin, unsigned* end) { use(); glUniform1uiv(location, std::distance(begin, end), begin); }
	//void program::uniform_1iv(unsigned location, int* begin, int* end) { use(); glUniform1iv(location, std::distance(begin, end), begin); }

	//
	void draw_arrays(primitive_type pt, unsigned start, unsigned count, std::shared_ptr<program> prog) {
		prog->use();
		glDrawArrays(pt, start, count);
	}
	void draw_arrays(primitive_type pt, unsigned start, unsigned count, std::shared_ptr<program> prog,
		std::shared_ptr<vertex_array> vao) {

		vao->bind();
		draw_arrays(pt, start, count, prog);
	}
	void draw_arrays(primitive_type pt, unsigned start, unsigned count, std::shared_ptr<program> prog,
		std::shared_ptr<vertex_array> vao, std::map<const unsigned, texture*> texture_units) {

		for (std::pair<unsigned, texture*> p : texture_units) {
			active_texture(*p.second, p.first);
		}

		draw_arrays(pt, start, count, prog, vao);
	}

	void active_texture(texture& tex, unsigned index) {
		glActiveTexture(GL_TEXTURE0 + index);
		tex.bind();
	}

	void debug_message_callback(
		void(f)(message_source source, message_type type, unsigned id, message_severity severity, unsigned length,
			const char* message, const void* userParam)) {
		glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(f), nullptr);
	}

	void clear_color(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void clear(std::initializer_list<clear_buffer> mask) {
		unsigned r = 0;
		for (auto x : mask)
			r |= x;
		glClear(r);
	}

	void viewport(int x, int y, unsigned w, unsigned h) {
		glViewport(x, y, w, h);
	}

	void vertex_attrib2fv(unsigned index, const float * values) {
		glVertexAttrib2fv(index, values);
	}
	void vertex_attrib4fv(unsigned index, const float * values) {
		glVertexAttrib4fv(index, values);
	}
}