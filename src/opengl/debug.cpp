#include "debug.hpp"
#include "GL/glew.h"

void gl::internal::debug_message_callback(debug_callback callback, const void *user_param) {
	glDebugMessageCallback((GLDEBUGPROC)callback, user_param);
}