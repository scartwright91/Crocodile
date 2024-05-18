#pragma once

#include "col/BoundingBox.h"

namespace Crocodile
{
    namespace s2d
    {

		enum TileShape {
			RECT,
			SLOPE_LEFT, // collision logic not implemented
			SLOPE_RIGHT // collision logic not implemented
		};

		struct Tile {
			float x;
			float y;
			float size;
			bool collideable;
			TileShape shape = RECT;
			inline col::BoundingBox getBoundingBox() const { return col::BoundingBox(x * size, y * size, size, size, 0.0f); }
		};

    }
}
