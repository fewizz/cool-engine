#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <audio.hpp>
#include "unsafe_iostream_operations.hpp"

namespace wav {
	enum class format_tag : uint16_t {
		pcm = 0x0001,
		ieee_float = 0x0003,
		alaw = 0x0006,
		mulaw = 0x0007,
		extensible = 0xfffe
	};

	class decoder {
		std::istream& istream;

		struct data_format {
			format_tag tag;
			uint16_t channels;
			uint32_t sample_rate;
			uint32_t data_rate;
			uint16_t data_block_size;
			uint16_t bits_per_sample;
		};
		data_format fmt;

	public:
		decoder(std::istream& is): istream{ is } {
			istream.ignore(16); // skip "RIFF", size, "WAVE", "fmt "

			uint32_t fmt_size = estd::get<uint32_t>(istream);
			if (fmt_size != 16)
				throw std::runtime_error("unsupported format");

			fmt = estd::get<data_format>(istream);
		}

		unsigned channels() { return fmt.channels; }
		unsigned bits_per_sample() { return fmt.bits_per_sample; }
		unsigned sample_rate() { return fmt.sample_rate; }

		void next_samples(std::vector<uint8_t> vec) {
			istream.ignore(4); // skip "data"
			uint32_t size = estd::get<uint32_t>(istream);
			char* arr = new char(size);
			vec.insert(vec.end(), arr, arr + size);
			delete[] arr;
			
			if (size & 1 == 1)
				istream.ignore(1);
		}

		void all_samples(std::vector<uint8_t> vec) {
			while (istream)
				next_samples(vec);
		}
	};
}