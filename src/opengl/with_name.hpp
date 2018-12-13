#pragma once
#include <memory>
#include "internal.hpp"

namespace gl {
	class with_name;

	namespace internal {
		unsigned get_name(with_name& obj);
	}

	class with_name {
		friend unsigned internal::get_name(with_name& obj);
		template<class T>
		friend std::shared_ptr<T> internal::view(T&);

		template<class T>
		friend std::shared_ptr<T> internal::view(unsigned);

	protected:
		static constexpr unsigned invalid_name = std::numeric_limits<unsigned>::max();

		void invalidate_name() {
			name = invalid_name;
		}
	public:
		with_name() {}
		with_name(unsigned name) :name{ name } {}
		with_name(with_name&& o) :name{ o.name } {
			o.invalidate_name();
		}

		with_name& operator=(with_name&& o) {
			name = o.name;
			o.invalidate_name();
			return *this;
		}

		unsigned name{ invalid_name };
	};

	namespace internal {
		inline unsigned get_name(with_name& obj) {
			return obj.name;
		}
	}
}