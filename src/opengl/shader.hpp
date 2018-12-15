#pragma once
#include "with_name.hpp"
#include <string>

namespace gl {
	namespace internal {
		unsigned create_shader(unsigned type);
		void delete_shader(unsigned shader);
		void shader_source(unsigned shader, unsigned count, const char* const* string, const int* length);
		void compile_shader(unsigned shader);

		enum shader_type : unsigned {
			compute_shader = 0x91B9,
			vertex_shader = 0x8B31,
			fragment_shader = 0x8B30
		};
	}

	class program;

	class shader :public with_name {
		friend program;
	protected:
		internal::shader_type type;

		shader(internal::shader_type type):with_name(internal::create_shader(type)), type{ type } {}

		shader(internal::shader_type type, std::string src):shader(type) {
			source(src);
			compile();
		}
	public:
		shader(shader&) = delete;
		shader& operator=(shader&) = delete;
		shader(shader&& s) :with_name{ std::move(s) } { type = s.type; }

		~shader() {
			if (name != invalid_name) {
				internal::delete_shader(name);
				invalidate_name();
			}
		}

		void source(std::string src) {
			const char* c_str = src.c_str();
			internal::shader_source(name, 1, &c_str, nullptr);
		}

		shader& compile() {
			internal::compile_shader(name);
			return *this;
		}
	};
	class vertex_shader : public shader {
	public:
		vertex_shader(std::string src) :shader(internal::shader_type::vertex_shader, src) {}
	};
	class fragment_shader : public shader {
	public:
		fragment_shader(std::string src) :shader(internal::shader_type::fragment_shader, src) {}
	};
}