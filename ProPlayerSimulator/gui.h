#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include "renderable.h"
#include "updatable.h"


namespace gui {
	class interface_element : public updatable, public renderable {};

	class with_position {
	public:
		virtual glm::vec2 get_pos() = 0;
	};

	class with_changeable_position :public with_position {
	public:
		virtual void set_pos(glm::vec2) = 0;
	};

	class with_position_state :public virtual with_changeable_position {
	protected:
		glm::vec2 pos_state;
	public:
		glm::vec2 get_pos() override { return pos_state; }
		void set_pos(glm::vec2 pos) override { pos_state = pos; }
	};

	/*class interface_element_with_childs :public interface_element {
		std::vector<interface_element> childs;
	public:
		std::vector<interface_element>& childs() { return childs; }

		void update() override {
			for (auto &c : childs)
				c.update();
		}

		void render() override {
			for (auto &c : childs)
				c.render();
		}
	};*/

	class pressable {
	public:
		virtual void press() {};
	};

	class with_dimesion {
		virtual glm::vec2 get_dim() = 0;
	};

	class with_changeable_dimension :public with_dimesion {
		virtual void set_dim(glm::vec2) = 0;
	};

	class with_dimension_state :public with_changeable_dimension {
	protected:
		glm::vec2 dim_state;
	public:
		glm::vec2 get_dim() override { return dim_state; }
		void set_dim(glm::vec2 dim) override { dim_state = dim; }
	};

	class unclaspable {
		virtual void unclasp() {};
	};

	class rectangle_button
		: public interface_element, 
		public with_position, public with_dimesion, 
		public pressable,
		public unclaspable
	{
		bool is_pressed{ false };
	public:
		void press() override { is_pressed = true; }
		void unclasp() override { is_pressed = false; }
	};
}