#pragma once

#include "freetype.hpp"
#include "renderer.hpp"
#include <memory>
#include <string>
#include "utf8.h"

class text_renderer : public verticies_renderer {
	std::vector<gl::texture_2d*> glyph_textures;
	std::string text;
	std::vector<int> textures_array_texture_units;
public:
	text_renderer(std::string str, freetype::face& face, std::shared_ptr<gl::program> program)
		:verticies_renderer(program, gl::primitive_type::triangles), text{str} {

		std::vector<float> positions;
		std::vector<unsigned> tex_units;
		std::map<uint32_t, gl::texture_2d*> char_tex;
		std::map<gl::texture_2d*, unsigned> tex_unit_index;
		std::vector<float> uvs;
		long penX = 0;
		long penY = 0;
		unsigned last_index = 0;

		for (auto begin = str.begin(); begin != str.end();) {
			uint32_t code_point = utf8::next(begin, str.end());

			if (code_point == '\n') {
				penY -= face.get_size_metrics().height();
				penX = 0;
				continue;
			}

			freetype::glyph glyph{ face.load_glyph(face.get_char_index(code_point)) };
			auto bitmap = glyph.get_bitmap();

			freetype::glyph::metrics metrics = glyph.get_metrics();
			float scaleX = 1.0 / 64.0;
			float scaleY = 1.0 / 64.0;

			float left = (penX + metrics.horizontal_bearing_x()) * scaleX;
			float right = (penX + metrics.horizontal_bearing_x() + metrics.width()) * scaleX;
			float bot = (penY + metrics.horizontal_bearing_y() - metrics.height()) * scaleY;
			float top = (penY + metrics.horizontal_bearing_y()) * scaleY;

			positions.insert(positions.end(),
				{
					left, bot,
					right, bot,
					right, top,
					left, bot,
					right, top,
					left, top
				});
			uvs.insert(uvs.end(),
				{
					0, 1,
					1, 1,
					1, 0,
					0, 1,
					1, 0,
					0, 0
				});

			if (char_tex[code_point] == nullptr) {
				auto glyph_tex = new gl::texture_2d;

				int w = bitmap.width();
				int h = bitmap.rows();
				std::vector<uint8_t> data(w * h * 4);

				for (unsigned x = 0; x < w; x++)
					for (unsigned y = 0; y < h; y++) {
						uint8_t c = bitmap.data<uint8_t>()[x + bitmap.pitch() * y];
						data[(x + y * w) * 4] = c;
						data[(x + y * w) * 4 + 1] = c;
						data[(x + y * w) * 4 + 2] = c;
						data[(x + y * w) * 4 + 3] = 1;
					}
				if (data.size() == 0) {
					data.insert(data.end(), { 0, 0, 0, 0 });
					w = h = 1;
				}

				glyph_tex->image(gl::internal_format::rgba8, w, h, gl::pixel_format::rgba, data);
				char_tex[code_point] = glyph_tex;
				tex_unit_index[glyph_tex] = last_index++;
				glyph_textures.push_back(glyph_tex);
			}
			unsigned unit = tex_unit_index[char_tex[code_point]];
			tex_units.insert(tex_units.end(), 6, unit);

			penX += metrics.horizontal_advance();
		}

		vertex_attrib_data(program->attrib_location("a_position"), gl::vertex_attribute::size{ 2 }, gl::vertex_attribute::normalized{ false }, positions);
		vertex_attrib_data(program->attrib_location("a_uv"), gl::vertex_attribute::size{ 2 }, gl::vertex_attribute::normalized{ false }, uvs);
		vertex_attrib_i_data(program->attrib_location("a_texture_unit"), gl::vertex_attribute::size{ 1 }, tex_units);

		textures_array_texture_units = std::vector<int>( glyph_textures.size() );
		for (int i = 0; i < glyph_textures.size(); i++)
			textures_array_texture_units[i] = i;
	}

	void render() override {
		update_matricies_uniforms();
		unsigned location = program->unifrom_location("u_textures");
		program->uniform_1iv(location, textures_array_texture_units);
		draw_arrays(pt, 0, 6 * text.length(), program, vao_, glyph_textures);
	}
};