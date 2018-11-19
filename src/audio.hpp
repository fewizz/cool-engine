#pragma once
#include <memory>
#include <istream>
#include <memory>

namespace audio {

	/*class sample_array {
		unique_ptr<char[]> data;
		size_t size;
	public:
		sample_array(unique_ptr<char[]> data, size_t size) :data{ data }, size{size} {}
		size_t size() {
			return size;
		}
		char& operator*() {
			return *data;
		}
	};*/

	class sample_provider {
	public:
		virtual void get(char* ptr, size_t size) = 0;
	};
}