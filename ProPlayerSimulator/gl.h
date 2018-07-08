#ifndef GL_WRAP_H
#define GL_WRAP_H

#include "GL/glew.h"
#include <exception>
#include <string>
#include <initializer_list>

namespace gl {
	class with_name {
	public:
		GLuint name;
	};

	class bindable:virtual public with_name {
	protected:
		virtual void bind() = 0;
	};
	class gennable:virtual public with_name {
	protected:
		virtual void gen() = 0;
	};
	class creatable:virtual public with_name {
	protected:
		virtual void create() = 0;
	};

	class vertex_array;
	class program;
	class shader;
	class texture;

	class context {
	public:
		/*void bind_texture_unit(texture tex, GLuint index) {
			tex.bind();
			glActiveTexture(tex.target);
		}*/
	};

	context* wrap_context() {
		return new context();
	}

	enum buffer_target : GLenum {
		array_buffer = GL_ARRAY_BUFFER
	};
	enum buffer_usage : GLenum {
		static_draw = GL_STATIC_DRAW
	};

	class buffer : public gennable, public bindable {
		friend vertex_array;
	protected:
		const buffer_target target;
		void bind() override { glBindBuffer(target, name); }
		void gen() override { glGenBuffers(1, &name); }
	public:
		void data(size_t bytes, const void * data, buffer_usage usage) { 
			bind();
			glBufferData(target, bytes, data, usage);
		}

		template<class T>
		void data(std::initializer_list<T> lst, buffer_usage usage) {
			data(sizeof(T)*lst.size(), lst.begin(), usage);
		}

		buffer(buffer_target tar):target{tar} {
			gen();
			bind();
		}
	};

	class vertex_array : public gennable, public bindable {
		void gen() override { glGenVertexArrays(1, &name); }
	public:
		void bind() override { glBindVertexArray(name); }
		vertex_array() {
			gen();
			bind();
		}

		void vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, buffer buff/*, std::initializer_list<T>data*/) {
			bind();
			buff.bind();
			glVertexAttribPointer(index, size, type, normalized, 0, nullptr);
		}
		void enable_vertex_attrib_array(GLuint index) {
			bind();
			glEnableVertexAttribArray(index);
		}
	};

	enum internal_format:GLint {
		RGBA8 = GL_RGBA8
	};

	enum pixel_format:GLenum {
		RGBA = GL_RGBA
	};

	enum pixel_type:GLenum {
		UBYTE = GL_UNSIGNED_BYTE
	};

	class texture : public gennable, public bindable {
	public:
		void bind() override {glBindTexture(target, name);}

		texture(GLenum _target):target{_target} {
			gen();
			bind();
		}
		GLenum target;
	protected:
		void gen() override {glGenTextures(1, &name);}
	};

	class texture2d : public texture {

	public:
		texture2d():texture(GL_TEXTURE_2D){}

		void image(internal_format if_, GLsizei w, GLsizei h, pixel_format pf, pixel_type pt, void* pntr) {
			bind();
			glTexImage2D(GL_TEXTURE_2D, 0, if_, w, h, 0, pf, pt, pntr);
		}

		void sub_image(GLint level, GLint xoff, GLint yoff, GLsizei w, GLsizei h, pixel_format pf, pixel_type pt, void* pntr) {
			bind();
			glTexSubImage2D(GL_TEXTURE_2D, level, xoff, yoff, w, h, pf, pt, pntr);
		}

		void storage(GLsizei levels, internal_format if_, GLsizei w, GLsizei h) {
			bind();
			glTexStorage2D(GL_TEXTURE_2D, levels, if_, w, h);
		}
	};

	class texture_rect :public texture2d {
	public:
		texture_rect() :texture2d() {}
	};

	class shader_compilation_error : public std::exception {
		friend shader;

		shader_compilation_error(shader*const s, std::string message) :shader_obj{ s }, std::exception(message.c_str()) {}
	public:
		shader * const shader_obj;
	};

	enum shader_type :GLenum {
		compute_shader = GL_COMPUTE_SHADER,
		vertex_shader = GL_VERTEX_SHADER,
		fragment_shader = GL_FRAGMENT_SHADER
	};

	class shader:public creatable {
		friend program;
	public:
		const shader_type type;
		shader(shader_type type):type{type} {create();}
		shader(shader_type type, std::string src) :shader(type) { source(src); compile(); }

		void source(std::string src) {
			const char* s { src.c_str() };
			glShaderSource(name, 1, &s, nullptr);
		}

		void compile() { 
			glCompileShader(name);
			GLint i{};
			GLint* status{ &i };
			glGetShaderiv(name, GL_COMPILE_STATUS, status);
			if (*status == GL_FALSE) {
				GLint len;
				glGetShaderiv(name, GL_INFO_LOG_LENGTH, &len);
				char* mess = new char[len];
				glGetShaderInfoLog(name, len, nullptr, mess);
				throw shader_compilation_error(this, mess);
			}
		}
	protected:
		void create() override {name = glCreateShader(type);}
	};

	class program :public creatable {
	protected:
		void create() override { name = glCreateProgram(); }
	public:
		program() { create(); }
		program(std::initializer_list<std::pair<shader_type, char*>> init, 
			void(errorh)(shader_compilation_error error) = [](shader_compilation_error err) {})
		{
			create();
			try {
				for (auto p : init) {
					shader s(p.first, p.second);
					attach(s);
				}
			}
			catch (shader_compilation_error e) { errorh(e); }
			link();
		}
		void attach(shader& sh) { glAttachShader(name, sh.name); }
		void link() { glLinkProgram(name); }
		void use() { glUseProgram(name); }
		GLuint attrib_location(std::string attrib_name) { return glGetAttribLocation(name, attrib_name.c_str()); };
	};

	class framebuffer : public gennable, public bindable {
		framebuffer() {
			gen();
			bind();
		}
	};

	enum primitive_type :GLenum {
		triangles = GL_TRIANGLES
	};

	void draw_arrays(primitive_type pt, GLint start, GLsizei count, program prog,
		vertex_array vao, std::initializer_list<std::pair<GLuint, texture>> texture_units = {}) {

		prog.use();
		vao.bind();

		for (auto p : texture_units) {
			glActiveTexture(GL_TEXTURE0 + p.first);
			p.second.bind();
		}

		glDrawArrays(pt, start, count);

	}
}


#endif