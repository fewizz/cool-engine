#pragma once
#include <vector>
#include <cmath>
#include <iterator>
#include <memory>
#include "opengl/gl.hpp"

class renderer {
protected:
	std::shared_ptr<gl::program> program;
public:
	explicit renderer(std::shared_ptr<gl::program> program) :program{ program } {}
	explicit renderer(gl::program&& program) :program{ new gl::program{std::move(program)} } {};
	renderer(const renderer&) = delete;
	renderer& operator=(const renderer&) = delete;

	virtual void render() = 0;

	virtual ~renderer() {}
};

class renderer_verticies : public renderer {
protected:
	std::shared_ptr<gl::vertex_array> vao_;
	gl::primitive_type pt;
	std::map<std::string, void*> matricies;

	void update_matricies_uniforms() {
		for (auto mat : matricies) {
			program->uniform_matrix4fv(program->unifrom_location(mat.first), 1, false, static_cast<float*>(mat.second));
		}
	}
public:
	renderer_verticies(std::shared_ptr<gl::program> program, gl::primitive_type pt)
		:renderer(program), pt{ pt }, vao_{ std::make_shared<gl::vertex_array>() } {}

	renderer_verticies(gl::program&& program, gl::primitive_type pt)
		:renderer(std::move(program)), 
		vao_{std::make_shared<gl::vertex_array>()}, pt{pt} {}

	renderer_verticies(gl::program&& program, gl::vertex_array&& vao, gl::primitive_type pt)
		:renderer(std::move(program)),
		vao_{ std::make_shared<gl::vertex_array>(std::move(vao)) }, pt{ pt } {}

	std::shared_ptr<gl::vertex_array> vao() {
		return vao_;
	}

	template<class Matrix>
	void matrix(std::string name, Matrix matrix) {
		matricies[name] = &matrix;
	}

	template <class C>
	void vertex_attrib_data(gl::vertex_attribute::location location, gl::vertex_attribute::size size,
		gl::vertex_attribute::normalized normalized, C container) {
		vertex_attrib_data(location, size, normalized, container.begin(), container.end());
	}

	template <class C>
	void vertex_attrib_i_data(gl::vertex_attribute::location location, gl::vertex_attribute::size size, C container) {
		vertex_attrib_i_data(location, size, container.begin(), container.end());
	}

	template <class RAI>
	void vertex_attrib_data(gl::vertex_attribute::location location, gl::vertex_attribute::size size,
		gl::vertex_attribute::normalized normalized, RAI begin, RAI end) {
		gl::array_buffer buff{};
		buff.data(begin, end);
		vao_->attrib_pointer<std::iterator_traits<RAI>::value_type>(location, size, buff, normalized);
		vao_->enable_attrib_array(location);
	}

	template <class RAI>
	void vertex_attrib_i_data(gl::vertex_attribute::location location, gl::vertex_attribute::size size, RAI begin, RAI end) {
		gl::array_buffer buff{};
		buff.data(begin, end);
		vao_->attrib_i_pointer<std::iterator_traits<RAI>::value_type>(location, size, buff);
		vao_->enable_attrib_array(location);
	}

	void render() override {
		update_matricies_uniforms();
		gl::draw_arrays(pt, 0, (unsigned)(vao_->attrib_buffer(0)->size() / vao_->attrib_size(0)), program, vao_);
	}
};