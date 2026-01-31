#pragma once

#include "r2/Node.hpp"
#include "rs/Display.hpp"

namespace r2 {
	namespace tools {
		class Scaler {
		public:
			static void reset(r2::Node* src);

			static void scale(double factor, r2::Node* src);

			static void centeredScale(r::Vector2 refSize, double factor, r2::Node* src) {
				src->scaleY = src->scaleX = factor;
				src->x = rs::Display::width() * 0.5 - refSize.x * 0.5 * factor;
				src->y = rs::Display::height() * 0.5 - refSize.y * 0.5 * factor;
			}

			static void topLeftContain(r::Vector2 srcSize, r::Vector2 destSize, r2::Node* src);

			static void centeredBox(
				r::Vector2		srcSize,
				r::Vector2		destSize,
				r2::Node* src);

			static void centeredContain(
				r::Vector2		srcSize,
				r::Vector2		destSize,
				r2::Node* src);

			static void fill(
				r::Vector2		srcSize,
				r::Vector2		destSize,
				r2::Node* src);
		};
	}
}