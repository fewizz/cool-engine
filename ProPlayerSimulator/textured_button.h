#pragma once

#include "gui.h"
#include "gl.h"
#include "glm/vec2.hpp"

class textured_button : public gui::rectangle_button<glm::vec2, glm::vec2>{
	gl::texture2d tex;

	textured_button() {

	}

	virtual void render() override {};
};