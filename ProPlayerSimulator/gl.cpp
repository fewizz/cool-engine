#define GLEW_STATIC
#include "GL/glew.h"
#include "gl.h"
#include <exception>

namespace gl {
	context* wrap_context() {
		glewInit();
		return new context{};
	}

	// buffer
	void buffer::gen() { glGenBuffers(1, name); }
	void buffer::gl_bind(unsigned target, unsigned name) { glBindBuffer(target, name); }
	void buffer::del() { glDeleteBuffers(1, name); delete name; }

	void buffer::data(size_t bytes, const void * data, buffer_usage usage) {
		bind();
		glBufferData(target, bytes, data, usage);
	}

	// vertex_array
	void vertex_array::gen() { glGenVertexArrays(1, name); }
	void vertex_array::bind() { glBindVertexArray(*name); }
	void vertex_array::del() { glDeleteBuffers(1, name); delete name; }

	void vertex_array::gl_vertex_attrib_pointer(unsigned index, unsigned size, unsigned type, bool normalized, unsigned stride, void* pointer) {
		glVertexAttribPointer(index, size, type, normalized, stride, nullptr);
	}

	void vertex_array::enable_vertex_attrib_array(unsigned index) {
		bind();
		glEnableVertexAttribArray(index);
	}

	// texture
	void texture::gen() { glGenTextures(1, name); }
	void texture::bind() { glBindTexture(target, *name); }
	void texture::del() { glDeleteBuffers(1, name); delete name; }

	// texture2d
	void texture2d::gl_tex_sub_image_2d(unsigned target, unsigned level, unsigned xo, unsigned yo, unsigned w, unsigned h, unsigned format, unsigned type, void* p) {
		glTexSubImage2D(texture_target::texture_2d, level, xo, yo, w, h, format, type, p);
	}

	template<class T>
	void texture2d::image(internal_format if_, unsigned w, unsigned h, pixel_format pf, std::vector<T> data) {
		bind();
		glTexImage2D(texture_target::texture_2d, 0, if_, w, h, 0, pf, gl_type_token<T>, data.data());
	}

	void texture2d::storage(unsigned levels, internal_format if_, unsigned w, unsigned h) {
		bind();
		glTexStorage2D(texture_target::texture_2d, levels, if_, w, h);
	}

	// sampler
	void sampler::gen() { glGenSamplers(1, name); }
	void sampler::del() { glDeleteSamplers(1, name); }

	void sampler::texture_wrap_s(wrap_mode wm) {glSamplerParameteri(*name, GL_TEXTURE_WRAP_S, wm); }
	void sampler::texture_wrap_t(wrap_mode wm) {glSamplerParameteri(*name, GL_TEXTURE_WRAP_T, wm); }
	void sampler::texture_wrap_r(wrap_mode wm) { glSamplerParameteri(*name, GL_TEXTURE_WRAP_R, wm); }
	// shader
	void shader::create() { *name = glCreateShader(type); }
	void shader::del() { glDeleteShader(*name); delete name; }

	void shader::source(std::string src) {
		const char* s { src.c_str() };
		glShaderSource(*name, 1, &s, nullptr);
	}

	void shader::compile() {
		glCompileShader(*name);
		GLint i{};
		GLint* status{ &i };
		glGetShaderiv(*name, GL_COMPILE_STATUS, status);
		if (*status == GL_FALSE) {
			//GLint len;
			glGetShaderiv(*name, GL_INFO_LOG_LENGTH, status);
			char* mess = new char[*status];
			glGetShaderInfoLog(*name, *status, nullptr, mess);
			throw shader_compilation_error(this, mess);
		}
	}

	// program
	void program::create() { *name = glCreateProgram(); }
	void program::del() { glDeleteProgram(*name); delete name; }

	void program::attach(shader& sh) { glAttachShader(*name, *sh.name); }
	void program::link() { glLinkProgram(*name); }
	void program::use() { glUseProgram(*name); }
	unsigned program::attrib_location(std::string attrib_name) { return glGetAttribLocation(*name, attrib_name.c_str()); };

	//

	void draw_arrays(primitive_type pt, unsigned start, unsigned count, program& prog,
		vertex_array& vao, std::initializer_list<std::pair<unsigned, texture*>> texture_units) {

		prog.use();
		vao.bind();

		for (std::pair<unsigned, texture*> p : texture_units) {
			glActiveTexture(GL_TEXTURE0 + p.first);
			p.second->bind();
		}

		glDrawArrays(pt, start, count);

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
}