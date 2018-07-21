#pragma once
#include "gl.h"
#include <vector>

//class renderer_combined;

class renderer {
public:
	renderer() = default;
	renderer(const renderer&) = delete;
	renderer operator=(const renderer&) = delete;
	virtual void render() {}
};
//////////////////////////////////////////

class renderer_combinable:public renderer {
public:
	virtual bool combinable(renderer other) { return false; };
};
//////////////////////////////////////////
class renderer_combined : public renderer_combinable {
	std::vector<renderer&> vec;
public:
	renderer_combined() {}
	void render() override { for (auto &&r : vec) r.render(); }

	/*renderer_combined operator+(const renderer & other) {
		renderer_combined o{ *this };
		o.vec.push_back(other);
		return o;
	}*/

	renderer_combined& operator+=(renderer r) {
		vec.push_back(r);
	}

	/*renderer_combined& operator+=(renderer_combined r) {
		for (auto r0 : r.vec) vec.push_back(r0);
		return *this;
	}*/
};
/////////////////////////////////////////
class renderer_with_program: public renderer {
	const gl::program& p;
public:
	renderer_with_program(const gl::program& pr) :p{ pr } {}
};
/////////////////////////////////////////
class quad_renderer :public renderer_with_program {

};