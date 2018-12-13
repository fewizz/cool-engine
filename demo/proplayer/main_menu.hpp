#pragma once
#include "gui.hpp"
#include "memory"
#include "renderer.hpp"
#include "text_renderer.hpp"
#include <string>


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

	main_menu(freetype::face& face)
		:start{ 50, 100, 200, 50, face, u8"Новая игра" },
		options{ 50, 0, 200, 50, face, u8"Настройки" },
		exit{ 50, -100, 200, 50, face, u8"Выйти"} {}

	void render() override {
		start.render();
		options.render();
		exit.render();
	}

	void update() override {}

};