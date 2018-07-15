#define GLEW_STATIC
#include "GL/glew.h"
#include "gl.h"
#include <exception>

namespace gl {
	context* wrap_context() {
		glewInit();
		return new context{};
	}

	template<class T>
	unsigned type_token() {
		/*if (std::is_same<int, int8_t>::value) {
			return GL_BYTE;
		}*/
		//else throw std::exception("GL: can't find appr. type");
		switch (T) {
		case int8_t: return GL_BYTE;
		case uint8_t: return GL_UNSIGNED_BYTE;
		case int16_t: return GL_SHORT;
		case uint16_t: return GL_UNSIGNED_SHORT;
		case float: return GL_FLOAT;
		case double: return GL_DOUBLE;
		default: throw std::exception("GL: can't find appr. type");
		}
	}

	// buffer
	void buffer::gen() { glGenBuffers(1, name); }
	void buffer::bind() { glBindBuffer(target, *name); }
	void buffer::del() { glDeleteBuffers(1, name); name = nullptr; }

	void buffer::data(size_t bytes, const void * data, buffer_usage usage) {
		bind();
		glBufferData(target, bytes, data, usage);
	}

	// vertex_array
	void vertex_array::gen() { glGenVertexArrays(1, name); }
	void vertex_array::bind() { glBindVertexArray(*name); }
	void vertex_array::del() { glDeleteBuffers(1, name); name = nullptr; }

	template<class T, int size>
	void vertex_array::vertex_attrib_pointer(buffer& buff, unsigned index, bool normalized) {
		bind();
		buff.bind();
		glVertexAttribPointer(index, size, type_token<T>(), normalized, 0, nullptr);
	}

	void vertex_array::enable_vertex_attrib_array(unsigned index) {
		bind();
		glEnableVertexAttribArray(index);
	}

	// texture
	void texture::gen() { glGenTextures(1, name); }
	void texture::bind() { glBindTexture(target, *name); }
	void texture::del() { glDeleteBuffers(1, name); name = nullptr; }

	// texture2d
	template<class T>
	void texture2d::image(internal_format if_, unsigned w, unsigned h, pixel_format pf, std::vector<T> data) {
		bind();
		glTexImage2D(texture_target::texture_2d, 0, if_, w, h, 0, pf, type_token<T>, data.data());
	}

	template<class T>
	void texture2d::sub_image(unsigned level, unsigned xoff, unsigned yoff, unsigned w, unsigned h, pixel_format pf, std::vector<T> data) {
		glBindBuffer(buffer_target::pixel_pack_buffer, 0);
		bind();
		glTexSubImage2D(texture_target::texture_2d, level, xoff, yoff, w, h, pixel_format, type_token<T>, data.data());
	}

	template<class T>
	void texture2d::sub_image(buffer& buff, unsigned level, unsigned xoff, unsigned yoff, unsigned w, unsigned h, pixel_format pf) {
		bind();
		buff.bind();
		glTexSubImage2D(texture_target::texture_2d, level, xoff, yoff, w, h, pf, type_token<T>, nullptr);
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
	void shader::create() { GLuint tname = glCreateShader(type); name = &tname; }
	void shader::del() { glDeleteShader(*name); name = nullptr; }

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
			GLint len;
			glGetShaderiv(*name, GL_INFO_LOG_LENGTH, &len);
			char* mess = new char[len];
			glGetShaderInfoLog(*name, len, nullptr, mess);
			throw shader_compilation_error(this, mess);
		}
	}

	// program
	void program::create() { GLuint tname = glCreateProgram(); name = &tname; }
	void program::del() { glDeleteProgram(*name); name = nullptr; }

	void program::attach(shader& sh) { glAttachShader(*name, *sh.name); }
	void program::link() { glLinkProgram(*name); }
	void program::use() { glUseProgram(*name); }
	unsigned program::attrib_location(std::string attrib_name) { return glGetAttribLocation(*name, attrib_name.c_str()); };

	//

	void draw_arrays(primitive_type pt, unsigned start, unsigned count, program prog,
		vertex_array vao, std::initializer_list<std::pair<unsigned, texture>> texture_units) {

		prog.use();
		vao.bind();

		for (auto p : texture_units) {
			glActiveTexture(GL_TEXTURE0 + p.first);
			p.second.bind();
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

	void clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}
}