#pragma once

namespace gl {
	namespace internal {
		void clear(unsigned mask);
		void clear_color(float r, float g, float b, float a);
		void viewport(int x, int y, unsigned w, unsigned h);
	}

	inline void clear_color(float r, float g, float b, float a) {
		internal::clear_color(r, g, b, a);
	}

	enum class clear_buffer : unsigned {
		color = 0x00004000,
		depth = 0x00000100
	};

	template<class... Mask>
	void clear(Mask... masks) {
		static_assert((std::is_same_v<gl::clear_buffer, Mask> && ...));
		internal::clear((unsigned)(masks | ...));
	}

	inline void viewport(int x, int y, unsigned w, unsigned h) {
		internal::viewport(x, y, w, h);
	}
}