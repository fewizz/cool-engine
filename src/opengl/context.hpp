#pragma once

namespace gl {

	namespace internal {
		void init();
	}

	class context {
	public:
	};

	inline context wrap_context() {
		gl::internal::init();
		return {};
	}
}