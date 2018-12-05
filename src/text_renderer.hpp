#pragma once

#include "freetype/freetype.hpp"
#include "renderer.hpp"
#include <memory>
#include <string>
#include "utf8.h"
#include "fixed_texture_atlas.hpp"
#include <algorithm>
#include <cmath>

namespace gfx {
	class text_renderer : public verticies_renderer {
		gfx::fixed_texture_atlas tex_atlas;
		std::string text;
		std::vector<int> textures_array_texture_units;

		static int calculate_max_size(freetype::face& face) {
			freetype::bbox global_box{ face.get_bbox() };
			unsigned side_size = std::max(global_box.x_max - global_box.x_min, global_box.y_max - global_box.x_min);
			return ceil(side_size / 64);
		}
	public:

		text_renderer(std::string str, freetype::face& face, std::shared_ptr<gl::program> program)
			:verticies_renderer(program, gl::primitive_type::triangles), text{ str }, tex_atlas{calculate_max_size(face), 30} {

			std::vector<float> positions;
			std::vector<float> uvs;

			std::map<uint32_t, std::pair<unsigned, unsigned>> char_uv;
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
				face.load_glyph(face.get_char_index(code_point));
				freetype::glyph_slot glyph{ face.get_glyph() };
				auto bitmap = glyph.get_bitmap();

				freetype::glyph_slot::metrics metrics = glyph.get_metrics();
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

				if (char_uv.find(code_point) == char_uv.end()) {
					unsigned w = bitmap.width();
					unsigned h = bitmap.rows();
					std::vector<uint8_t> data(w * h * 4);

					for (unsigned x = 0; x < w; x++) {
						for (unsigned y = 0; y < h; y++) {
							uint8_t c = bitmap.data<uint8_t>()[x + bitmap.pitch() * y];
							data[(x + y * w) * 4] = c;
							data[(x + y * w) * 4 + 1] = c;
							data[(x + y * w) * 4 + 2] = c;
							data[(x + y * w) * 4 + 3] = 1;
						}
					}
					if (data.size() == 0) {
						data.insert(data.end(), { 0, 0, 0, 0 });
						w = h = 1;
					}

					std::pair<unsigned, unsigned> right_bot = tex_atlas.add(w, h, data.data());
					char_uv[code_point] = right_bot;
				}

				std::pair<unsigned, unsigned> p = char_uv[code_point];
				float x = p.first / (float)tex_atlas.width();
				float y = p.second / (float)tex_atlas.height();
				float w = metrics.width() / 64.0 / (float)tex_atlas.width();
				float h = metrics.height() / 64.0 / (float)tex_atlas.height();

				uvs.insert(uvs.end(),
					{
						x, y+h,
						x+w, y+h,
						x+w, y,
						x, y+h,
						x+w, y,
						x, y
					});

				penX += metrics.horizontal_advance();
			}

			vertex_attrib_data(program->attrib_location("a_position"), gl::vertex_attribute::size{ 2 }, gl::vertex_attribute::normalized{ false }, positions);
			vertex_attrib_data(program->attrib_location("a_uv"), gl::vertex_attribute::size{ 2 }, gl::vertex_attribute::normalized{ false }, uvs);
		}

		void render() override {
			update_matricies_uniforms();
			gl::active_texture(tex_atlas, 0);
			program->uniform_1i(program->unifrom_location("u_atlas"), 0);
			draw_arrays(pt, 0, 6 * text.length(), program, vao_);
		}
	};
}