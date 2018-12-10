#pragma once
#include <memory>

namespace gl {
	namespace internal {
		template<class T>
		std::shared_ptr<T> view(unsigned name) {
			T* ptr = new T(name);
			return std::shared_ptr<T>(ptr, [](T* p) {p->invalidate_name();  delete p; });
		}

		template<class T>
		std::shared_ptr<T> view(T& t) {
			return view<T>(t.name);
		}

		template<class T>
		static unsigned constexpr type_token() {
			if (std::is_same_v<T, int8_t>)
				return 0x1400;
			if (std::is_same_v<T, uint8_t>)
				return 0x1401;
			if (std::is_same_v<T, int32_t>)
				return 0x1404;
			if (std::is_same_v<T, uint32_t>)
				return 0x1405;
			if (std::is_same_v<T, float_t>)
				return 0x1406;
			else throw std::exception("GL: can't find appr. type");
		}
	}
}