#pragma once
#include <cinttypes>
#include <string>

namespace riff {

	class chunk {
	protected:
		uint32_t chunk_id;
		uint32_t chunk_size;
		void* data;
	};

	class chunk_form {
		virtual std::string get_id() = 0;
	};

	class riff_form :public chunk_form {
		std::string get_id() override { return "RIFF"; }
	};
}