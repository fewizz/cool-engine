#pragma once

#include "freetype/freetype.hpp"
#include "renderer.hpp"
#include <memory>
#include <string>
#include "utf8.h"
#include "fixed_texture_atlas.hpp"
#include <algorithm>
#include <cmath>
#include <map>
#include "opengl/core.hpp"

namespace gfx {
	class text_renderer : public verticies_renderer {
		gfx::fixed_texture_atlas tex_atlas;
		std::string text;
		std::vector<int> textures_array_texture_units;
		std::vector<gl::array_buffer> buffers;
		float width = 0;

		static int calculate_max_size(freetype::face& face) {
			freetype::bbox global_box{ face.get_bbox() };
			unsigned side_size = std::max(global_box.x_max - global_box.x_min, global_box.y_max - global_box.y_min);
			return (unsigned) ceil(side_size / 64.0);
		}
	public:
		//text_renderer(std::string str, freetype::face& face, gl::program& program)
		//	:text_renderer(str, face, std::make_shared{program}) {}


		text_renderer(std::string str, freetype::face& face, std::shared_ptr<gl::program> program)
			:verticies_renderer(program), text{ str }, tex_atlas{ calculate_max_size(face), 30 } {
			tex_atlas.mag_filter(gl::mag_filter::nearest);
			tex_atlas.min_filter(gl::min_filter::nearest);

			std::vector<float> positions;
			std::vector<float> uvs;

			std::map<uint32_t, std::pair<unsigned, unsigned>> char_uv;
			long penX = 0;
			long penY = 0;
			unsigned last_index = 0;

			float scaleX = 1.0 / 64.0;
			float scaleY = 1.0 / 64.0;

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
							data[(x + y * w) * 4] = 0xFF;
							data[(x + y * w) * 4 + 1] = 0xFF;
							data[(x + y * w) * 4 + 2] = 0xFF;
							data[(x + y * w) * 4 + 3] = c;
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
				float w = metrics.width() / 64.0f / (float)tex_atlas.width();
				float h = metrics.height() / 64.0f / (float)tex_atlas.height();

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
			width = penX * scaleX;

			buffers.push_back(gl::array_buffer(positions));
			vertex_array->attrib_pointer<float>(program->get_attrib_location("a_position"), gl::vertex_attribute::size{ 2 }, buffers.back());
			vertex_array->enable_attrib_array(program->get_attrib_location("a_position"));

			buffers.push_back(gl::array_buffer(uvs));
			vertex_array->attrib_pointer<float>(program->get_attrib_location("a_uv"), gl::vertex_attribute::size{ 2 }, buffers.back());
			vertex_array->enable_attrib_array(program->get_attrib_location("a_uv"));
		}

		void render() override {
			gl::enable_blending();
			gl::blend_func(gl::blending_factor::src_alpha, gl::blending_factor::one_minus_src_alpha);
			gl::active_texture(tex_atlas, 0);
			program->uniform<int>(program->get_unifrom_location("u_atlas"), 0);
			program->draw_arrays(gl::primitive_type::triangles, 0, 6 * (unsigned)text.length(), *vertex_array);
		}

		float get_width() {
			return width;
		}
	};
}