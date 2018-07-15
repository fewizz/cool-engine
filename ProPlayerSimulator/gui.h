#pragma once
#include <vector>

namespace gui {

	class interface_element {
	public:
		virtual void update() = 0;
		virtual void render() = 0;
	};

	template<class Pos>
	class with_position {
		virtual Pos get_pos() = 0;
	};

	template<class Pos>
	class with_changeable_position:public with_position<Pos> {
		virtual void set_pos() = 0;
	};

	template<class Pos>
	class positioned_interface_element:public interface_element, public with_position<Pos> {
	public:
	};

	class screen:public interface_element {
		std::vector<interface_element> childs;

	public:
		std::vector<interface_element>& getChilds() { return childs; }

		void update() {
			for (auto &c : childs)
				c.update();
		}

		void render() {
			for (auto &c : childs)
				c.render();
		}
	};

	class pressable {
	public:
		virtual void press() {};
	};

	template<class Dim>
	class with_dimesion {
		virtual Dim get_dim() = 0;
	};

	template<class Dim>
	class with_changeable_dimension:with_dimesion{
		virtual void set_dim(Dim d) = 0;
	};

	class unclaspable {
		virtual void unclasp() {};
	};

	template<class Pos, class Dim>
	class rectangle_button : public positioned_interface_element<Pos>, public pressable, public unclaspable, public with_dimension<Dim> {
		bool is_pressed{ false };
	public :
		void press() override { is_pressed = true; }
		void unclasp() override { is_pressed = false; }
	};
}