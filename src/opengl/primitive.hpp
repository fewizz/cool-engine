#pragma once

namespace gl {
	enum primitive_type :unsigned {
		lines = 1,
		line_loop,
		line_strip,
		triangles,
		triangle_strip,
		triangle_fan
	};
}