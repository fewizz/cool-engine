#pragma once
#include <vector>
#include <cmath>
#include <iterator>
#include <memory>
#include "opengl/gl.hpp"
#include <functional>
#include <array>
#include <glm/mat4x4.hpp>

namespace gfx {

	/*class program_validator {
		std::shared_ptr<gl::program> program;

	public:
		program_validator() {}
		program_validator(std::shared_ptr<gl::program> program) :program{ program } {}

		void pre_render_call() {
		}

		void post_render_call() {
		}
	};*/

	class renderer {
	protected:
		//std::shared_ptr<gl::program> program;
	public:
		//renderer();
		//explicit renderer(std::shared_ptr<gl::program> program) :program{ program } {}
		//explicit renderer(gl::program&& program) :program{ new gl::program{std::move(program)} } {};
		//renderer(const renderer&) = delete;
		//renderer& operator=(const renderer&) = delete;

		virtual void render() = 0;

		//virtual void render(gl::primitive_type pt, size_t count, unsigned start = 0) {
		//	gl::draw_arrays(pt, start, count, *program);
		//};

		virtual ~renderer() {}
	};



	/*template<class... Types>
	struct vertex_attributes {
		Types... types;
	};

	template<class... Types>
	struct vertex_data : vertex_data<Types...> {
		std::tuple< std::vector<Types>... > vectors;

		vertex_data(vertex_attributes<HeadType, Types...> attribs):vertex_data(T) {

		}
	};*/



	/*class verticies_renderer : public renderer {
	protected:
		std::shared_ptr<gl::vertex_array> vao_;
		gl::primitive_type pt;
		std::map<std::string, std::function<glm::mat4(void)>> matrix_providers;
		std::vector<std::unique_ptr<gl::array_buffer>> buffers;

		void update_matricies_uniforms() {
			for (auto mat : matrix_providers) {
				program->uniform_matrix4fv(program->unifrom_location(mat.first), 1, false, (float*)&mat.second());//static_cast<float*>(mat.second()));
			}
		}
	public:
		verticies_renderer(std::shared_ptr<gl::program> program, gl::primitive_type pt)
			:renderer(program), pt{ pt }, vao_{ std::make_shared<gl::vertex_array>() } {}

		verticies_renderer(gl::program&& program, gl::primitive_type pt)
			:renderer(std::move(program)),
			vao_{ std::make_shared<gl::vertex_array>() }, pt{ pt } {}

		verticies_renderer(gl::program&& program, gl::vertex_array&& vao, gl::primitive_type pt)
			:renderer(std::move(program)),
			vao_{ std::make_shared<gl::vertex_array>(std::move(vao)) }, pt{ pt } {}

		std::shared_ptr<gl::vertex_array> vao() {
			return vao_;
		}

		template<class MatrixProvider>
		void matrix(std::string name, MatrixProvider matrix_provider) {
			matrix_providers[name] = matrix_provider;
		}

		//template<class... Types>
		//void vertex_data(vertex_data<Types...> data) {
		//}

		template <class C>
		void vertex_attrib_data(gl::vertex_attribute::location location, gl::vertex_attribute::size size,
			gl::vertex_attribute::normalized normalized, const C& container) {
			vertex_attrib_data(location, size, normalized, container.begin(), container.end());
		}

		template <class C>
		void vertex_attrib_i_data(gl::vertex_attribute::location location, gl::vertex_attribute::size size, const C& container) {
			vertex_attrib_i_data(location, size, container.begin(), container.end());
		}

		template <class RAI>
		void vertex_attrib_data(gl::vertex_attribute::location location, gl::vertex_attribute::size size,
			gl::vertex_attribute::normalized normalized, RAI begin, RAI end) {
			auto buff = std::make_unique<gl::array_buffer>();
			buff->data(begin, end);
			vao_->attrib_pointer<std::iterator_traits<RAI>::value_type>(location, size, *buff, normalized);
			vao_->enable_attrib_array(location);
			buffers.push_back(std::move(buff));
		}

		template <class RAI>
		void vertex_attrib_i_data(gl::vertex_attribute::location location, gl::vertex_attribute::size size, RAI begin, RAI end) {
			auto buff = std::make_unique<gl::array_buffer>();
			buff->data(begin, end);
			vao_->attrib_i_pointer<std::iterator_traits<RAI>::value_type>(location, size, *buff);
			vao_->enable_attrib_array(location);
			buffers.push_back(std::move(buff));
		}

		void render(gl::primitive_type pt, unsigned start, size_t count, gl::vertex_array& vao) {
			update_matricies_uniforms();
			gl::draw_arrays(pt, start, count, program, vao);
		}
	};

	/*class textured_verticies_renderer : public verticies_renderer {
		std::vector<std::pair<unsigned, std::shared_ptr<gl::texture>>> textures;

	public:
		using verticies_renderer::verticies_renderer;

		void texture(unsigned location, std::shared_ptr<gl::texture> tex) {
			textures.push_back({ location, tex });
		}

		void render() override {
			for (std::pair<unsigned, std::shared_ptr<gl::texture>> p : textures) {
				active_texture(*p.second, p.first);
				program->uniform_1i(p.first, 0);
			}

			verticies_renderer::render();
		}
	};*/

}
/*template<class Vec2>
class square {
	std::array<Vec2, 4> squares;
public:
	square(Vec2 v0, Vec2 v1, Vec2 v2, Vec2 v3) :squares{ v0, v1, v2, v3 } {}
};

template<class Vec2>
class squares_renderer : public verticies_renderer {
public:
	std::unique_ptr<gl::element_array_buffer> eabo;

	template<class RAI>
	squares_renderer(gl::program&& prog) :verticies_renderer(prog, gl::primitive_type::triangles) {
		
	}


};*/