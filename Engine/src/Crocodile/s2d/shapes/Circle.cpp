#include "Circle.h"

namespace Crocodile
{
    namespace s2d
    {
        namespace shapes
        {
            Circle::Circle(float radius) : radius(radius)
            {
                renderMethod = "circle";
                size = glm::vec2(2 * radius);
            }
        }
    }
}