#pragma once
#include <functional>
#include <string>

namespace gl {
	namespace internal {
		typedef void (debug_callback)(unsigned source, unsigned type, unsigned id,
			unsigned severity, unsigned length, const char *message,
			const void *user_param);

		void debug_message_callback(debug_callback, const void *user_param);
	}

	enum message_source {
	};

	enum message_type {
		error = 0x824C,
		deprecated_behavior,
		undefined_behavior,
		portability,
		performance,
		marker,
		push_group,
		pop_group,
		other
	};

	enum message_severity {
	};

	void debug_message_callback(internal::debug_callback callback, const void *user_param = nullptr) {
		internal::debug_message_callback(callback, user_param);
	}

	namespace debug_internal {
		thread_local std::function<void(std::string message)> callback_message;
		thread_local std::function<void(message_type type, std::string message)> callback_message_with_type;
	}

	void debug_message_callback(void(callback)(std::string message), const void *user_param = nullptr) {
		debug_internal::callback_message = callback;

		internal::debug_message_callback([](unsigned source, unsigned type, unsigned id,
			unsigned severity, unsigned length, const char *message,
			const void *user_param) {

			debug_internal::callback_message(std::string(message, length));
			
		}, user_param);
	}

	void debug_message_callback(void(callback)(message_type type, std::string message), const void *user_param = nullptr) {
		debug_internal::callback_message_with_type = callback;

		internal::debug_message_callback([](unsigned source, unsigned type, unsigned id,
			unsigned severity, unsigned length, const char *message,
			const void *user_param) {

			debug_internal::callback_message_with_type((message_type)type, std::string(message, length));

		}, user_param);
	}
}