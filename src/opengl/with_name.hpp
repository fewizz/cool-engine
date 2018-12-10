#pragma once
#include <memory>

namespace gl {
	class with_name {
		template<class T>
		friend std::shared_ptr<T> view(T&);

		template<class T>
		friend std::shared_ptr<T> view(unsigned);
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
}