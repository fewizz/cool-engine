#include "context.hpp"

gl::context wrap_context() {
	gl::internal::init();
	return gl::context();
}