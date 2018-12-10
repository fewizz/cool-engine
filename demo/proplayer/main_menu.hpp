#pragma once
#include "gui.hpp"
#include "memory"
#include "renderer.hpp"
#include "text_renderer.hpp"
#include "opengl/gl.hpp"
#include <string>

class main_menu_button_renderer : public gfx::verticies_renderer {
public:
	using gfx::verticies_renderer::verticies_renderer;

	void render() override;
};
class main_menu_button : public gui::pressable, public gui::square_view {
	static std::shared_ptr<main_menu_button_renderer> renderer;
	std::shared_ptr<gfx::text_renderer> text_renderer;
	static std::shared_ptr<gl::program> text_program;
	static gl::vertex_attribute::location pos_loc;
	gl::array_buffer vbo;
	float x, y, w, h;
public:
	~main_menu_button();

	main_menu_button(float x, float y, float w, float h, freetype::face& face, std::string str);

	void render() override;
	void update() override {};
	void press() override {};
	void unpress() override {};
};


class main_menu : public gui::square_view {
public:
	main_menu_button start, options, exit;

	main_menu(freetype::face& face)
		:start{ 50, 100, 200, 50, face, u8"Новая игра" },
		options{ 50, 0, 200, 50, face, u8"Настройки" },
		exit{ 50, -100, 200, 50, face, u8"Выйти"
	} {}

	void render() override {
		start.render();
		options.render();
		exit.render();
	}

	void update() override {}

};