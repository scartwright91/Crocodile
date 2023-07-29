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

				glm::vec2 center = glm::vec2(0.f);
				float rotation = 0.0;

				BoundingBox()
				{
				}

				BoundingBox(float x, float y, float width, float height, float rotation)
				{
					this->x = x;
					this->y = y;
					this->width = width;
					this->height = height;
					this->xMin = x;
					this->xMax = x + width;
					this->yMin = y;
					this->yMax = y + height;
					this->center = glm::vec2(x + width / 2, y + height / 2);
					this->rotation = rotation;
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
					// rotate vertices around bbox center
					if (rotation != 0.0)
					{
						for (unsigned int i = 0; i < 4; i++)
						{
							glm::vec2 p = vertices[i];
							p.x -= center.x;
							p.y -= center.y;
							float newX = p.x * glm::cos(rotation) - p.y * glm::sin(rotation);
							float newY = p.y * glm::cos(rotation) + p.x * glm::sin(rotation);
							vertices[i] = glm::vec2(newX + center.x, newY + center.y);
						}
					}
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
