#pragma once
#include <string>
#include <istream>
#include <vector>

namespace freetype {
	class face;
	class glyph;

	namespace internal {
		#define FT_ERRORDEF_(e, v, s) e = v,
		#define FT_NOERRORDEF_(e, v, s) e = v,
		enum error {
			#include "freetype/fterrdef.h"
		};

		#define FT_ERRORDEF_(e, v, s) if( code == v ) return s ;
		#define FT_NOERRORDEF_(e, v, s) if( code == v ) return s ;
		inline std::string get_error_name(unsigned code) {
			#include "freetype/fterrdef.h"
			throw std::runtime_error("undefined freetype error code");
		}
		#undef FT_ERRORDEF_
		#undef FT_NOERRORDEF_

		void init();
		freetype::face&& load(void* begin, size_t size);
	}

	template<class RAI>
	face&& load(RAI begin, RAI end) {
		return face{ internal::load(&*begin, end - begin) };
	}

	template<class Container>
	face&& load(Container& c) {
		return face{ internal::load(&*c.begin(), c.end() - c.begin()) };
	}

	typedef unsigned glyph_index;

	class glyph {
		friend face;
		void* ft_glyph_;

	protected:
		glyph(void* glyph) :ft_glyph_{glyph} {}
	public:
		class bitmap {
			void* ft_bitmap;
			void* ft_data();
		public:
			bitmap(void* original) :ft_bitmap{ original } {}
			template<class T>
			T* data() { return static_cast<T*>(ft_data()); }
			unsigned rows();
			unsigned width();
			unsigned pitch();
		};
		class metrics {
			friend glyph;
			void* ft_metrics;
			metrics(void* ptr) : ft_metrics{ ptr } {}
		public:
			long width();
			long height();
			long horizontal_bearing_x();
			long horizontal_bearing_y();
			long horizontal_advance();

			long vertical_bearing_x();
			long vertical_bearing_y();
			long vertical_advance();
		};
	public:
		glyph(const glyph&& g)
			:ft_glyph_{ g.ft_glyph_ } {}

		void render();
		bitmap get_bitmap();
		metrics get_metrics();

		int bitmap_left();
		int bitmap_top();
	};

	class face {
		friend glyph;
		friend face&& internal::load(void*, size_t);
		void* ft_face_;
	protected:
		face(void* face) : ft_face_{ face } {}
	public:
		class size_metrics {
			friend face;
			void* ft_size_metrics_;
			size_metrics(void* data) :ft_size_metrics_{ data } {}
		public:
			unsigned short x_ppem();
			unsigned short y_ppem();
			signed long height();
		};

		face(face&& f) :ft_face_{
			f.ft_face_
		} {}
		face&& operator=(face&& f) {
			ft_face_ = f.ft_face_;
		}

		void set_char_size(unsigned w, unsigned h, unsigned hdr, unsigned wdr);
		glyph_index get_char_index(unsigned charcode);
		glyph&& load_glyph(glyph_index index);
		size_metrics get_size_metrics();
		short height();
		unsigned short units_per_EM();
	};

	extern face&& load_from_stream(std::istream& stream);
}