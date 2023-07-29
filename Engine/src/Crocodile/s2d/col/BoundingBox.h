#pragma once

#include "../../Core.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

namespace Crocodile
{
	namespace s2d
	{
		namespace col
		{

			class Vector
			{
			public:
				float x = 0.f;
				float y = 0.f;
				Vector(float x, float y)
				{
					this->x = x;
					this->y = y;
				};
				void rotate(float rotation)
				{
					float newX = x * glm::cos(rotation) - y * glm::sin(rotation);
					float newY = y * glm::cos(rotation) + x * glm::sin(rotation);
					this->x = newX;
					this->y = newY;
				}
			};

			class CROCODILE_API BoundingBox
			{

			public:
				float x = 0.0;
				float y = 0.0;
				float width = 0.0;
				float height = 0.0;
				float xMin = 0.0f;
				float xMax = 0.0f;
				float yMin = 0.0f;
				float yMax = 0.0f;

				glm::vec2 center = glm::vec2(0.f);
				float rotation = 0.0;

				BoundingBox();
				BoundingBox(float x, float y, float width, float height, float rotation);

				// collision / intersection
				bool intersectsBounds(BoundingBox b);
				bool intersectsPoint(glm::vec2 p);

				// transformation
				void inflate(float dx, float dy, bool centre);

				// vertices
				std::vector<glm::vec2> getVertices();
				void printVertices();
				std::vector<glm::vec2> getAxes();

			private:
				bool intersectsRotatedBounds(BoundingBox b);
			};
		}
	}
}
