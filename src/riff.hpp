#pragma once
#include <cinttypes>
#include <string>
#include <istream>
#include <stack>
#include "unsafe_iostream_operations.hpp"

namespace riff {

	struct chunk_info {
		uint32_t id;
		uint32_t size;
		operator bool() {
			return (uint64_t(id) << 32 || uint64_t(size));
		}
	};

	class chunk {
	protected:
		chunk_info ci;
		void* data;
	};

	class chunk_visitor {
		std::istream& stream;
		const std::streampos start;
		chunk_info current{ 0, 0 };

	public:
		chunk_visitor(std::istream& stream) :stream{ stream } {
			if (stream.eof())
				current = {0, 0};
			else {
				current = { estd::get<uint32_t>(stream), estd::get<uint32_t>(stream) };
			}
		}

		operator bool() {
			return current;
		}

		chunk_visitor next() {
			stream.ignore();
			return chunk_visitor{ stream };
		}

		/*template<class T>
		T get() {
			int size = sizeof(T);
			char* bytes = new char[size];
			std::streampos prev = stream.tellg();
			stream.read(bytes, size);
			stream.seekg(prev);
			return { *(T*)bytes };
		}

		template<class T>
		T read() {
			int size = sizeof(T);
			char* bytes = new char[size];
			stream.read(bytes, size);
			return { *(T*)bytes };
		}*/

		chunk_visitor next_from_data() {
			return chunk_visitor(stream);
		}
	};

	/*class chunk_form {
		virtual std::string get_id() = 0;
	};

	class riff_form :public chunk_form {
		std::string get_id() override { return "RIFF"; }
	};*/

	chunk_info next_chunk_info( std::istream& istream ) {

	}
}