#pragma once
#include "glm/vec2.hpp"
#include "gl.h"

class textured_rectangle {
	glm::vec2 pos;
	glm::uvec2 dim;
	gl::texture_2d& tex;

public:
	textured_rectangle(gl::texture_2d& texture, glm::vec2 left_up_pos, glm::uvec2 dimension) :tex{texture}, pos { left_up_pos }, dim{ dimension } {}

	void render() {
	}
};