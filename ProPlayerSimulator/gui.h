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
	class with_changeable_position:public virtual with_position<Pos> {
		virtual void set_pos() = 0;
	};

	template<class Pos>
	class with_position_state:public virtual with_position<Pos> {
	protected:
		Pos pos_state;
	public:
		virtual Pos get_pos() override { return pos_state; }
	};

	class screen:public interface_element {
		std::vector<interface_element> childs;

	public:
		std::vector<interface_element>& childs() { return childs; }

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
	class with_changeable_dimension:public virtual with_dimesion<Dim>{
		virtual void set_dim(Dim d) = 0;
	};

	template<class Dim>
	class with_dimension_state:public virtual with_dimesion<Dim> {
	protected:
		Dim dim_state;
	public:
		virtual Dim get_dim() override { return dim_state; }
	};

	template<class Dim>
	class with_changeable_dimension_state:public virtual with_dimension_state<Dim>, public virtual with_changeable_dimension<Dim> {
	public:
		virtual void set_dim(Dim d) override { dim_state = d; }
	};

	class unclaspable {
		virtual void unclasp() {};
	};

	template<class Pos, class Dim>
	class rectangle_button : public virtual interface_element, public virtual with_position<Pos>, public pressable, public unclaspable, public virtual with_dimesion<Dim> {
		bool is_pressed{ false };
	public :
		void press() override { is_pressed = true; }
		void unclasp() override { is_pressed = false; }
	};
}