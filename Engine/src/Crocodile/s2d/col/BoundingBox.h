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

			class Line
			{
			public:
				std::string axis;
				glm::vec2 origin;
				glm::vec2 direction;
				Line(std::string axis, glm::vec2 origin, glm::vec2 direction)
				{
					this->axis = axis;
					this->origin = origin;
					this->direction = direction;
				};
			};

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
				float getMagnitude()
				{
					return glm::sqrt(x * x + y * y);
				};
				void rotate(float rotation)
				{
					float newX = x * glm::cos(rotation) - y * glm::sin(rotation);
					float newY = y * glm::cos(rotation) + x * glm::sin(rotation);
					this->x = newX;
					this->y = newY;
				}
				void add(glm::vec2 v)
				{
					x += v.x;
					y += v.y;
				}
				void minus(glm::vec2 v)
				{
					x -= v.x;
					y -= v.y;
				}
				void multiply(glm::vec2 v)
				{
					x *= v.x;
					y *= v.y;
				}
				void project(Line line)
				{
					float dot = line.direction.x * (x - line.origin.x) + line.direction.y * (y - line.origin.y);
					x = line.origin.x + line.direction.x * dot;
					y = line.origin.y + line.direction.y * dot;
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

				std::vector<glm::vec2> vertices = {};
				Vector xAxis = Vector(1.f, 0.f);
				Vector yAxis = Vector(0.f, 1.f);

				BoundingBox();
				BoundingBox(float x, float y, float width, float height, float rotation);

				// collision / intersection
				bool intersectsBounds(BoundingBox b);
				bool intersectsPoint(glm::vec2 p);

				// transformation
				void inflate(float dx, float dy, bool centre);

				// vertices
				void printVertices();
				std::vector<Line> getAxes();
				std::vector<glm::vec2> getDebugAxes();

			private:
				std::vector<glm::vec2> getVertices();
				bool intersectsRotatedBounds(BoundingBox b);
			};
		}
	}
}
