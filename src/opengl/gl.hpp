#pragma once

#include <exception>
#include <string>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <map>
#include <array>
#include <memory>
#include <type_traits>

namespace gl {

	class context {
	public:
	};

	context wrap_context();

	enum class clear_buffer: unsigned {
		color = 0x00004000,
		depth = 0x00000100
	};
	void clear_color(float r, float g, float b, float a);

	namespace internal {
		void clear(unsigned mask);
	}

	template<class... Mask>
	void clear(Mask... masks) {
		static_assert((std::is_same_v<gl::clear_buffer, Mask> && ...) );
		internal::clear((unsigned)( masks | ... ));
	}

	void viewport(int x, int y, unsigned w, unsigned h);
}