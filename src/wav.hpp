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
	class wav_audio_data_provider : public audio::audio_data_provider {
		IStream& stream;
		uint32_t wave_chunks_bytes;

		struct {
			format_tag tag;
			uint16_t channels;
			uint32_t sample_rate;
			uint32_t data_rate;
			uint16_t data_block_size;
			uint16_t bits_per_sample;
			uint16_t size_of_extension;
			uint16_t number_of_valid_bits;
			uint32_t speaker_position_mask;
		} data_format;

		void* sub_data;
	public:
		wav_audio_data_provider(IStream& stream) :stream{stream} {
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

			stream.read((char*)(&data_format), chunk_size);
			//stream.ignore(16);
		}

		audio::audio_data next() override {
			typedef std::char_traits<char> ch_traits;
			char temp_4[4];

			stream.read(temp_4, 4);
			if (ch_traits::compare(temp_4, "data", 4) != 0)
				throw std::exception("invalid data");

			uint32_t size;
			stream.read((char*)&size, 4);

			auto data = std::make_unique<char[]>(size);
			stream.read(data.get(), size);

			if (size % 2 != 0)
				stream.ignore(1);

			return std::move(data);
		}
	};
}