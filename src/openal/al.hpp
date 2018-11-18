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
		void* alc_context_ptr_;
		context(void* alc_context_ptr_) :alc_context_ptr_{ alc_context_ptr_ } {}
	};

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
	class buffer : public with_name {
		static unsigned al_gen();
	public:
		enum class format {
			mono8 = 0x1100,
			mono16,
			stereo8,
			stereo16
		};
		buffer():with_name(al_gen()) {}

		template<class RAI>
		void data(format fmt, RAI begin, RAI end, size_t frequency) {

		}
	};

	class source : public with_name {

	public:
		source():withc
	};
}