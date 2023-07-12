#pragma once

#include "../../Core.h"

namespace Crocodile
{
	namespace s2d
	{
		namespace col
		{
			class CROCODILE_API BoundingBox
			{

			public:
				float x = 0.0;
				float y = 0.0;
				float width = 0.0;
				float height = 0.0;

				float xMin = x;
				float xMax = x + width;
				float yMin = y;
				float yMax = y + height;

				BoundingBox()
				{
				}

				BoundingBox(float x, float y, float width, float height)
				{
					this->x = x;
					this->y = y;
					this->width = width;
					this->height = height;
					this->xMin = x;
					this->xMax = x + width;
					this->yMin = y;
					this->yMax = y + height;
				}

				~BoundingBox()
				{
				}

				bool intersectsBounds(BoundingBox b)
				{
					return !(xMin >= b.xMax || yMin >= b.yMax || xMax <= b.xMin || yMax <= b.yMin);
				}

				bool intersectsPoint(glm::vec2 p)
				{
					return (p.x <= xMax && p.x >= xMin && p.y <= yMax && p.y >= yMin);
				}

				void inflate(float dx, float dy, bool centre)
				{
					if (centre)
					{
						x -= dx / 2;
						y -= dy / 2;
					}
					else
					{
						x -= dx;
						y -= dy;
					}
					width += dx;
					height += dy;
					xMin = x;
					xMax = x + width;
					yMin = y;
					yMax = y + height;
				}

				std::vector<glm::vec2> getVertices()
				{
					std::vector<glm::vec2> vertices = {};
					vertices.push_back(glm::vec2(xMin, yMin));
					vertices.push_back(glm::vec2(xMax, yMin));
					vertices.push_back(glm::vec2(xMax, yMax));
					vertices.push_back(glm::vec2(xMin, yMax));
					return vertices;
				}

				void printVertices()
				{
					std::cout << "topleft: [" << (int)xMin << ", " << (int)yMin
							  << "]; topright: [" << (int)xMax << ", " << (int)yMin
							  << "]; bottomleft: [" << (int)xMin << ", " << (int)yMax
							  << "]; bottomright: [" << (int)xMax << ", " << (int)yMax
							  << "]" << std::endl;
				}
			};
		}
	}
}
