#include "freetype.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H
#include <exception>
#include <iostream>

// !!! Dangerous part !!!
#define ft_face (static_cast<FT_Face>(ft_face_))
#define ft_size_metrics (static_cast<FT_Size_Metrics*>(ft_size_metrics_))
#define ft_glyph (static_cast<FT_GlyphSlot>(ft_glyph_))
#define ft_bitmap (*static_cast<FT_Bitmap*>(ft_bitmap))
#define ft_metrics (*static_cast<FT_Glyph_Metrics*>(ft_metrics))

using namespace freetype;
using namespace std;

FT_Library lib;

/* Internal */

void check_for_errors(FT_Error error) {
	if (error != internal::Ok) {
		std::string error_name = internal::get_error_name(error);
		throw std::runtime_error("freetype error: " + error_name);
	}
}

void internal::init() {
	check_for_errors(FT_Init_FreeType(&lib));
}

face&& internal::load(void* begin, size_t size) {
	internal::init();
	FT_Face f;
	check_for_errors(FT_New_Memory_Face(lib, static_cast<FT_Byte*>(begin), size, 0, &f));
	return f;
}

/* Face */
void face::set_char_size(unsigned w, unsigned h, unsigned wdr, unsigned hdr) {
	check_for_errors(FT_Set_Char_Size(ft_face, w, h, wdr, hdr));
}

glyph_index face::get_char_index(unsigned charcode) {
	return FT_Get_Char_Index(ft_face, charcode);
}

glyph&& face::load_glyph(glyph_index index) {
	check_for_errors(FT_Load_Glyph(ft_face, index, FT_LOAD_RENDER));
	return glyph{ ft_face->glyph };
}

short face::height() {
	return ft_face->height;
}

unsigned short face::units_per_EM() {
	return ft_face->units_per_EM;
}

face::size_metrics face::get_size_metrics() {
	return size_metrics{ &ft_face->size->metrics };
}

/* Size Metrics */
unsigned short face::size_metrics::x_ppem() {
	return ft_size_metrics->x_ppem;
}
unsigned short face::size_metrics::y_ppem() {
	return ft_size_metrics->y_ppem;
}
signed long face::size_metrics::height() {
	return ft_size_metrics->height;
}

/* Glyph */
void freetype::glyph::render() {
	FT_Render_Glyph(ft_glyph, FT_RENDER_MODE_NORMAL);
}

glyph::bitmap glyph::get_bitmap() {
	return glyph::bitmap{ &(static_cast<FT_GlyphSlot>(ft_glyph_)->bitmap) };
}

glyph::metrics glyph::get_metrics() {
	return glyph::metrics{ &(static_cast<FT_GlyphSlot>(ft_glyph_)->metrics) };
}

int glyph::bitmap_left() {
	return ft_glyph->bitmap_left;
}

int glyph::bitmap_top() {
	return ft_glyph->bitmap_top;
}

/* Glyph Bitmap */
void* glyph::bitmap::ft_data() {
	return ft_bitmap.buffer;
}

unsigned glyph::bitmap::rows() {
	return ft_bitmap.rows;
}

unsigned glyph::bitmap::width() {
	return ft_bitmap.width;
}

unsigned glyph::bitmap::pitch() {
	return ft_bitmap.pitch;
}

/* Glyph Metrics */
long glyph::metrics::width() { return ft_metrics.width; }
long glyph::metrics::height() { return ft_metrics.height; }
long glyph::metrics::horizontal_bearing_x() { return ft_metrics.horiBearingX; }
long glyph::metrics::horizontal_bearing_y() { return ft_metrics.horiBearingY; }
long glyph::metrics::horizontal_advance() { return ft_metrics.horiAdvance; }

long glyph::metrics::vertical_bearing_x() { return ft_metrics.vertBearingX; }
long glyph::metrics::vertical_bearing_y() { return ft_metrics.vertBearingY; }
long glyph::metrics::vertical_advance() { return ft_metrics.vertAdvance; }

namespace freetype {
	face&& load_from_stream(std::istream& stream) {
		std::streampos prev = stream.tellg();
		stream.seekg(0, std::ios_base::end);
		std::streampos size = stream.tellg();
		stream.seekg(prev);
		char* bytes = new char[size]; // API Reference -> User Allocarion
		stream.read(bytes, size);
		return face{ load(bytes, bytes + size) };
	}
}