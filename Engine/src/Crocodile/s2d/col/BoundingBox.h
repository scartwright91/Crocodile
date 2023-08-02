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

			struct Line
			{
				std::string axis;
				glm::vec2 origin;
				glm::vec2 direction;
			};

			struct LineIntersection
			{
				Line axis;
				glm::vec2 min;
				glm::vec2 max;
				bool collision;
			};

			struct CollisionVectors
			{
				std::vector<Line> cornerProjections;
				std::vector<LineIntersection> lineIntersections;
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
				CollisionVectors getCollisionVectors(BoundingBox *b);

			private:
				std::vector<glm::vec2> getVertices();
				bool intersectsRotatedBounds(BoundingBox b);
				int getSign(glm::vec2 a, glm::vec2 b);
				float getMagnitude(glm::vec2 a, glm::vec2 b);
			};
		}
	}
}
