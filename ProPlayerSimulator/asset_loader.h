#pragma once

#include "gl.h"
#include "lodepng.h"
#include <vector>
#include <string>

namespace asset_loader {
	gl::texture_2d load_texture2d(std::string path) {
		std::vector<uint8_t> bts;
		unsigned w, h;

		if (lodepng::decode(bts, w, h, path))
			throw std::exception(("Error when loading texture with path: " + path).c_str());

		gl::texture_2d tex;
		tex.storage(1, gl::internal_format::rgba8, w, h);
		tex.sub_image(0, 0, 0, w, h, gl::pixel_format::rgba, bts.data());
		return tex;
	}
}