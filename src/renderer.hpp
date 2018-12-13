#pragma once
#include <vector>
#include <cmath>
#include <iterator>
#include <memory>
#include <functional>
#include <array>
#include "glm/mat4x4.hpp"
#include "opengl/program.hpp"
#include "opengl/vertex_array.hpp"
#include "opengl/texture.hpp"

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

	struct renderer {
		virtual void render() = 0;
	};

	//using renderer = renderable;

	class shader_renderer : public renderer {
	protected:
		std::shared_ptr<gl::program> program;
	public:
		explicit shader_renderer(std::shared_ptr<gl::program> program) :program { program } {}
		explicit shader_renderer(gl::program&& program):program { new gl::program{std::move(program)} } {};
		shader_renderer(const shader_renderer&) = delete;
		shader_renderer& operator=(const shader_renderer&) = delete;

		std::shared_ptr<gl::program> get_program() {
			return program;
		}

		virtual ~shader_renderer() {}
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



	class verticies_renderer : public shader_renderer {
	protected:
		std::shared_ptr<gl::vertex_array> vertex_array;
		//gl::primitive_type pt;
		//std::map<std::string, std::function<glm::mat4(void)>> matrix_providers;
		//std::vector<std::unique_ptr<gl::array_buffer>> buffers;

		//void update_matricies_uniforms() {
		//	for (auto mat : matrix_providers) {
		//		program_sptr->uniform_matrix4fv(program_sptr->unifrom_location(mat.first), 1, false, (float*)&mat.second());//static_cast<float*>(mat.second()));
		//	}
		//}
	public:
		verticies_renderer(std::shared_ptr<gl::program> program)
			:shader_renderer(program), vertex_array{ std::make_shared<gl::vertex_array>() } {}

		verticies_renderer(gl::program&& program)
			:shader_renderer(std::move(program)),
			vertex_array{ std::make_shared<gl::vertex_array>() } {}

		verticies_renderer(gl::program&& program, gl::vertex_array&& vao)
			:shader_renderer(std::move(program)),
			vertex_array{ std::make_shared<gl::vertex_array>(std::move(vao)) } {}

		std::shared_ptr<gl::vertex_array> get_vertex_array() {
			return vertex_array;
		}

		//template<class MatrixProvider>
		//void matrix(std::string name, MatrixProvider matrix_provider) {
		//	matrix_providers[name] = matrix_provider;
		//}

		/*template <class C>
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
		}*/
	};

	class rectangle_renderer : public verticies_renderer {
		gl::array_buffer positions;

	public:
		rectangle_renderer(float x, float y, float w, float h, std::shared_ptr<gl::program> program, gl::buffer_usage usage = gl::buffer_usage::static_draw)
			:verticies_renderer( program )
		{
			positions.data(std::vector<float>{
				x, y,
				x + w, y,
				x, y + h,
				x + w, y + h,
			}, usage);
			gl::vertex_attribute::location pos_loc = program->get_attrib_location("a_position");

			vertex_array->attrib_pointer<float>(pos_loc, 2, positions);
			vertex_array->enable_attrib_array(pos_loc);
		}

		rectangle_renderer(float x, float y, float w, float h, gl::program&& program, gl::buffer_usage usage = gl::buffer_usage::static_draw)
			:rectangle_renderer(x, y, w, h, std::make_shared<gl::program>(std::move(program))) {}

		rectangle_renderer(std::shared_ptr<gl::program> program)
			:rectangle_renderer( 0, 0, 0, 0, program, gl::buffer_usage::dynamic_draw) {}
		rectangle_renderer(gl::program&& program)
			:rectangle_renderer(std::make_shared<gl::program>(std::move(program))) {}

		void update(float x, float y, float w, float h) {
			positions.sub_data(0, std::vector<float>{
				x, y,
				x + w, y,
				x, y + h,
				x + w, y + h,
			});
		}

		void render() override {
			program->draw_arrays(gl::primitive_type::triangle_strip, 0, 4, *vertex_array);
		}
	};

	class textured_rectangle_renderer : public rectangle_renderer {
		std::shared_ptr<gl::texture_2d> texture_;
		gl::array_buffer uvs;
	public:
		using rectangle_renderer::rectangle_renderer;

		void texture(std::shared_ptr<gl::texture_2d> texture) {
			texture_ = texture;
		}

		void texture(gl::texture_2d&& texture, int xo, int yo, int w, int h) {
			float tw = texture.width();
			float th = texture.height();

			uvs.data(std::vector<float> {
				xo / tw, (yo + h) / th,
				(xo + w) / tw, (yo + h) / th,
				xo / tw, yo / th,
				(xo + w) / tw, yo / th
			});
			gl::vertex_attribute::location uv_loc = program->get_attrib_location("a_uv");
			vertex_array->attrib_pointer<float>(uv_loc, 2, uvs);
			vertex_array->enable_attrib_array(uv_loc);
			texture_ = std::make_shared<gl::texture_2d>(std::move(texture));
		}

		void render() override {
			gl::active_texture(*texture_, 0);
			program->uniform<int>(program->get_unifrom_location("u_texture"), 0);
			rectangle_renderer::render();
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