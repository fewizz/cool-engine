#include "openal/al.hpp"
#include "openal/alc.hpp"
#include "wav.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

using namespace std;
using namespace filesystem;

int main() {
	alc::device al_dev = alc::open_device();
	alc::context al_context = al_dev.create_context();
	alc::make_context_current(al_context);

	cout << al::internal::get_error() << "\n";

	al::buffer buff;
	al::source src;

	cout << al::internal::get_error() << "\n";

	path path("C:\\Program Files (x86)\\OpenAL 1.1 SDK\\samples\\media\\stereo.wav");
	ifstream stream(path, std::ios::binary);

	vector<uint8_t> bytes;
	wav::data_format fmt = wav::decode(stream, bytes);

	buff.data(fmt.channels, fmt.bits_per_sample, bytes, fmt.sample_rate);

	cout << al::internal::get_error() << "\n";

	src.buffer(buff);
	src.play();

	cout << al::internal::get_error() << "\n";

	cin.get();
}