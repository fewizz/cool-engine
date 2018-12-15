#include "debug.hpp"

namespace gl {
	namespace internal {
		static std::function<void(std::string message)> callback_message;
		static std::function<void(message_type type, std::string message)> callback_message_with_type;
	}
}

void gl::debug_message_callback(internal::debug_callback callback, const void *user_param) {
	internal::debug_message_callback(callback, user_param);
}

void gl::debug_message_callback(void(callback)(std::string message), const void *user_param) {
	internal::callback_message = callback;

	internal::debug_message_callback([](message_source source, message_type type, unsigned id,
		message_severity severity, unsigned length, const char *message,
		const void *user_param) {

		internal::callback_message(std::string(message, length));

	}, user_param);
}

void gl::debug_message_callback(void(callback)(message_type type, std::string message), const void *user_param) {
	internal::callback_message_with_type = callback;

	internal::debug_message_callback([](message_source source, message_type type, unsigned id,
		message_severity severity, unsigned length, const char *message,
		const void *user_param) {

		internal::callback_message_with_type((message_type)type, std::string(message, length));

	}, user_param);
}