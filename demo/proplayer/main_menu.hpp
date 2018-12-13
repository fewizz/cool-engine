#pragma once
#include "gui.hpp"
#include "memory"
#include "renderer.hpp"
#include "text_renderer.hpp"
#include <string>
#include <fstream>
#include "lodepng.h"
#include "opengl/shader.hpp"
#include <iostream>
#include "glm/ext/matrix_clip_space.hpp"

extern int w;
extern int h;

class main_menu_button : public gui::pressable, public gui::rectangle_view {
	std::shared_ptr<gfx::rectangle_renderer> rectangle_renderer;
	std::shared_ptr<gfx::text_renderer> text_renderer;

	static std::shared_ptr<gl::program> rectangle_program;
	static std::shared_ptr<gl::program> text_program;
	float x, y, w, h;
public:
	~main_menu_button();

	main_menu_button(float x, float y, float w, float h, freetype::face& face, std::string str);

	void render() override;
	void update() override {};
	void press() override {};
	void unpress() override {};
};


class main_menu : public gui::rectangle_view {
public:
	main_menu_button start, options, exit;
	gfx::textured_rectangle_renderer back;

	main_menu(freetype::face& face)
		:start{ 50, 100, 200, 50, face, u8"Новая игра" },
		options{ 50, 0, 200, 50, face, u8"Настройки" },
		exit{ 50, -100, 200, 50, face, u8"Выйти"},
		back{ gl::program {
			gl::vertex_shader {R"(
				#version 420 core
				uniform mat4 u_mat;
				in vec2 a_position;
				in vec2 a_uv;
				out vec2 uv;
				void main() {
					uv = a_uv;
					gl_Position = u_mat * vec4(a_position, 0, 1);
				}
			)"},
			gl::fragment_shader {R"(
				#version 420 core
				out vec4 color;
				in vec2 uv;
				uniform sampler2D u_texture;			

				void main() {
					color = texture(u_texture, uv);
				}
			)"}
		} }
	{
		std::ifstream png{ "E:/Repos/cool-engine/demo/proplayer/main.png", std::ios_base::binary };
		unsigned w, h;

		std::streampos prev = png.tellg();
		png.seekg(0, std::ios_base::end);
		size_t size = png.tellg();
		png.seekg(prev);

		std::vector<unsigned char> raw(size);
		std::vector<unsigned char> out;

		png.read((char*)raw.data(), size);

		unsigned error = lodepng::decode(out, w, h, raw);
		if (error) {
			std::cout << "lodepng error " << error << "\n";
		}

		gl::texture_2d tex;
		tex.image(gl::internal_format::rgba8, w, h, gl::pixel_format::rgba, out);
		tex.mag_filter(gl::mag_filter::nearest);
		tex.min_filter(gl::min_filter::nearest);

		back.texture(std::move(tex), 0, 0, w, h);
	}

	void render() override {
		back.get_program()->uniform_mat<4, 4, float>(back.get_program()->get_unifrom_location("u_mat"), &glm::ortho<float>(0, ::w, 0, ::h));
		back.update(0, 0, ::w, ::h);
		back.render();
		start.render();
		options.render();
		exit.render();
	}

	void update() override {}

};