#pragma once
#include <string>

namespace al {
	class context;
	class device;

	device&& open_device(std::string device_name = "");
	void make_context_current(context& c);

	class device {
		friend device&& open_device(std::string);
		void* alc_device_ptr_;
		device(void* alc_device_ptr_) :alc_device_ptr_{ alc_device_ptr_ } {}
	public:
		context&& create_context();
	};

	class context {
		friend context&& device::create_context();
		friend void make_context_current(context&);
		void* alc_context_ptr_;
		context(void* alc_context_ptr_) :alc_context_ptr_{ alc_context_ptr_ } {}
	};

	unsigned get_error();

	class with_name {
	protected:
		static constexpr unsigned invalid_name = std::numeric_limits<unsigned>::max();

		void invalidate_name() {
			name = invalid_name;
		}
		virtual void del() {}
	public:
		with_name(unsigned name) :name{ name } {}
		with_name(with_name&& o) :name{ o.name } {
			o.invalidate_name();
		}

		with_name& operator=(with_name&& o) {
			name = o.name;
			o.invalidate_name();
			return *this;
		}

		unsigned name{ invalid_name };
		virtual ~with_name() {
			if (name)
				del();
		}
	};
	namespace internal {
		enum source_param {
			source_relative = 0x202,
			cone_inner_angle = 0x1001,
			cone_outer_angle = 0x1002,
			looping = 0x1007,
			buffer = 0x1009,
			source_state = 0x1010
		};
	}
	class buffer : public with_name {
		static void al_gen(int n, unsigned* buffers);
		static unsigned gen() {
			unsigned name;
			al_gen(1, &name);
			return name;
		}
		static void al_data(unsigned name, unsigned format, void* data, int size, int frequency);
	public:
		enum class format {
			mono8 = 0x1100,
			mono16,
			stereo8,
			stereo16
		};
		buffer():with_name(gen()) {}

		template<class RAI>
		void data(format fmt, RAI begin, RAI end, int frequency) {
			al_data(name, static_cast<unsigned>(fmt), begin, std::distance(begin, end), frequency);
		}

		template<class RAI>
		void data(int channels, int bits, RAI begin, RAI end, int frequency) {
			data(static_cast<al::buffer::format>
				(0x1100 + (channels - 1) * 2 + bits / 8 - 1), begin, end, frequency);
		}
	};

	class source : public with_name {
		static void al_gen(int n, unsigned* sources);
		static unsigned gen() {
			unsigned name;
			al_gen(1, &name);
			return name;
		}
		static void al_i(unsigned source, unsigned param, int value);
	public:
		source() :with_name{ gen() } {}

		void buffer(al::buffer& buf) {
			al_i(name, internal::source_param::buffer, buf.name);
		}

		void play();
	};
}