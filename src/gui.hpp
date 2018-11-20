#pragma once

namespace gui {

	class view {
	public:
		virtual void update() = 0;
		virtual void render() = 0;
	};

	class square_view : public virtual view {
		/*enum origin_meaning {
			center,
			left_up,
			left_bot,
			right_up,
			right_bot
		};*/
	};

	/*class button : public view {
		virtual void press() = 0;
		virtual void unpress() = 0;
	};*/
}