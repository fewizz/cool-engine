#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <audio.hpp>

namespace wav {

	enum class format_tag : uint16_t {
		pcm = 0x0001,
		ieee_float = 0x0003,
		alaw = 0x0006,
		mulaw = 0x0007,
		extensible = 0xfffe
	};

	template<class IStream>
	class wave_sample_provider : public audio::sample_provider {
		IStream& stream;
		uint32_t wave_chunks_bytes;
		int16_t samples_to_read = -1;
		bool odd = false;
		//uint16_t last_size = 0;

		struct {
			format_tag tag;
			uint16_t channels;
			uint32_t sample_rate;
			uint32_t data_rate;
			uint16_t data_block_size;
			uint16_t bits_per_sample;
			//uint16_t size_of_extension;
			//uint16_t number_of_valid_bits;
			//uint32_t speaker_position_mask;
		} data_format;

		void* sub_data;
	public:
		uint16_t channels() { return data_format.channels; }
		uint16_t sample_rate() { return data_format.sample_rate; }
		uint16_t bits_per_sample() { return data_format.bits_per_sample; }
		uint16_t data_block_size() { return data_format.data_block_size; }

		wave_sample_provider(IStream& stream) :stream{ stream} {
			typedef std::char_traits<char> ch_traits;
			char temp_4[4];

			stream.read(temp_4, 4);
			if (ch_traits::compare(temp_4, "RIFF", 4) != 0)
				throw std::exception("invalid data");

			stream.read(temp_4, 4);
			wave_chunks_bytes = *((uint32_t*)temp_4) - 4;

			stream.read(temp_4, 4);
			if (ch_traits::compare(temp_4, "WAVE", 4) != 0)
				throw std::exception("invalid data");

			stream.read(temp_4, 4);
			if (ch_traits::compare(temp_4, "fmt ", 4) != 0)
				throw std::exception("invalid data");

			stream.read(temp_4, 4);
			auto chunk_size = *((uint32_t*)temp_4);
			if(chunk_size != 16)
				throw std::exception("unsupported");

			stream.read((char*)(&data_format), chunk_size);
		}

		void get(char* ptr, size_t size) override {
			typedef std::char_traits<char> ch_traits;

			if (to_read == -1) {
				char temp_4[4];

				stream.read(temp_4, 4);
				if (ch_traits::compare(temp_4, "data", 4) != 0)
					throw std::exception("invalid data");

				stream.read((char*)&samples_to_read, 4);
				odd = samples_to_read % 2 != 0;
			}

			stream.read(ptr, samples_to_read);
		}
	};
}