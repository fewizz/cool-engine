#pragma once

#include "gl.h"
#include <string>
#include "gl.h"

namespace game {
	namespace assets_loader {
		gl::texture_2d load_texture2d(std::string path);
	}
}