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

	void debug_message_callback(internal::debug_callback callback, const void *user_param = nullptr);
	void debug_message_callback(void(callback)(std::string message), const void *user_param = nullptr);
	void debug_message_callback(void(callback)(message_type type, std::string message), const void *user_param = nullptr);
}