#pragma once
#include <vector>

namespace gui {

	template<typedef T>
	class position {
	public:
		virtual T get() = 0;
	};

	template<typedef T>
	class relative_position : public virtual position<T> {
		position* base;


	};

	class interface_element {
	public:
		virtual void update() = 0;
		virtual void render() = 0;
	};

	template<typedef T>
	class positioned_interface_element:interface_element {
		position<T> pos;
	};

	class screen:public interface_element {
		std::vector<interface_element&> childs;

	public:
		std::vector<interface_element&>& getChilds() { return childs; }

		void update() {
			for (auto &c : childs)
				c.update();
		}

		void render() {
			for (auto &c : childs)
				c.render();
		}
	};

	class button : interface_element {
		bool active;


	};
}