#pragma once
#include <memory>
#include <istream>
#include <memory>

namespace audio {

	typedef std::unique_ptr<char[]> audio_data;

	class audio_data_provider {
	public:
		virtual audio_data next() = 0;
	};
}