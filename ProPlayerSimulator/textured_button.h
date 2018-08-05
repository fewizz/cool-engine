#pragma once

#include "gui.h"
#include "gl.h"
#include "glm/vec2.hpp"
#include "renderer.h"


namespace gui {
	class textured_button : public gui::rectangle_button {
		quad_renderer rend;

	public:
		textured_button(float w, float h, float x, float y)
			: rend{ {x, y}, {x, y + h}, {x + w, y + h}, {x + w, h} } {}

		void texture(gl::texture_2d* tex) {
			rend.texture(tex);
		}

		void render() override {
			rend.render();
		}
	};
}