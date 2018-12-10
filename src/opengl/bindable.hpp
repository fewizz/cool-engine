#pragma once

namespace gl {
	class bindable {
	protected:
		virtual void bind() = 0;
	};
}