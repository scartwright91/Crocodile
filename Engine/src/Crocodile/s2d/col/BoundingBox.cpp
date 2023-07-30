#include "BoundingBox.h"

namespace Crocodile
{
    namespace s2d
    {
        namespace col
        {
            BoundingBox::BoundingBox()
            {
                this->xMin = x;
                this->xMax = x + width;
                this->yMin = y;
                this->yMax = y + height;
                vertices = getVertices();
            }

            BoundingBox::BoundingBox(float x, float y, float width, float height, float rotation)
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
                vertices = getVertices();
                xAxis.rotate(rotation);
                yAxis.rotate(rotation);
            }

            bool BoundingBox::intersectsBounds(BoundingBox b)
            {
                if (rotation != 0.0)
                    return intersectsRotatedBounds(b);
                return !(xMin >= b.xMax || yMin >= b.yMax || xMax <= b.xMin || yMax <= b.yMin);
            }

            bool BoundingBox::intersectsPoint(glm::vec2 p)
            {
                return (p.x <= xMax && p.x >= xMin && p.y <= yMax && p.y >= yMin);
            }

            void BoundingBox::inflate(float dx, float dy, bool centre)
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

            std::vector<glm::vec2> BoundingBox::getVertices()
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

            std::vector<Line> BoundingBox::getAxes()
            {
                std::vector<Line> lines = {};
                lines.push_back(Line("x", center, glm::vec2(xAxis.x, xAxis.y)));
                lines.push_back(Line("y", center, glm::vec2(yAxis.x, yAxis.y)));
                return lines;
            }

            std::vector<glm::vec2> BoundingBox::getDebugAxes()
            {
                std::vector<glm::vec2> axes = {};
                glm::vec2 px1 = glm::vec2(
                    center.x + width * 0.75 * xAxis.x,
                    center.y + width * 0.75 * xAxis.y);
                glm::vec2 px2 = glm::vec2(
                    center.x - width * 0.75 * xAxis.x,
                    center.y - width * 0.75 * xAxis.y);
                glm::vec2 py1 = glm::vec2(
                    center.x + height * 0.75 * yAxis.x,
                    center.y + height * 0.75 * yAxis.y);
                glm::vec2 py2 = glm::vec2(
                    center.x - height * 0.75 * yAxis.x,
                    center.y - height * 0.75 * yAxis.y);
                axes.push_back(px1);
                axes.push_back(px2);
                axes.push_back(py1);
                axes.push_back(py2);
                return axes;
            }

            void BoundingBox::printVertices()
            {
                std::cout << "topleft: [" << (int)xMin << ", " << (int)yMin
                          << "]; topright: [" << (int)xMax << ", " << (int)yMin
                          << "]; bottomleft: [" << (int)xMin << ", " << (int)yMax
                          << "]; bottomright: [" << (int)xMax << ", " << (int)yMax
                          << "]" << std::endl;
            }

            bool BoundingBox::intersectsRotatedBounds(BoundingBox b)
            {
                // logic implemented from https://stackoverflow.com/questions/62028169/how-to-detect-when-rotated-rectangles-are-colliding-each-other
                // https://github.com/ArthurGerbelot/rect-collide
                bool collide = true;
                glm::vec2 limits = glm::vec2(0.f);
                for (Line line : b.getAxes())
                {
                    float rectHalfSize = (line.axis == "x" ? b.width : b.height) / 2;
                    for (glm::vec2 vertex : vertices)
                    {
                        Vector v(vertex.x, vertex.y);
                        v.project(line);
                        v.minus(center);
                        bool sign = (v.x * line.direction.x) + (v.y * line.direction.y) > 0;
                        float signedDistance = v.getMagnitude() * (sign ? 1 : -1);

                        if (limits.x > signedDistance)
                            limits.x = signedDistance;
                        if (limits.y < signedDistance)
                            limits.y = signedDistance;
                    }
                    if ((limits.x < 0 && limits.y > 0) || (glm::abs(limits.x) < rectHalfSize) || (glm::abs(limits.y) < rectHalfSize))
                        collide = false;
                }
                return collide;
            }

        }
    }
}