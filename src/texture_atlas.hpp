#pragma once
#include "opengl/gl.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

namespace gfx {

	class texture_atlas : public gl::texture_2d {
		class rectangle {
			int x0, y0, x1, y1;
		public:
			unsigned width() {
				return abs(x0 - x1);
			}
			unsigned height() {
				return abs(y0 - y1);
			}
			unsigned area() {
				return width()*height();
			}
		};

		std::vector<rectangle> rectangles;

	public:
		void add(rectangle rec) {
			std::sort(rectangles.begin(), rectangles.end(), [&](rectangle rec1, rectangle rec2) {
				return rec1.area() < rec2.area();
			});

			auto r_ptr = std::find_if(rectangles.begin(), rectangles.end(),
				[&](rectangle r) {return r.width() >= rec.width() && r.height() >= rec.height(); });
		}
	};
}