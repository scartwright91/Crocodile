#include "Circle.h"

namespace Crocodile
{
    namespace s2d
    {
        namespace shapes
        {
            Circle::Circle(float radius) : radius(radius)
            {
                m_renderMethod = "circle";
                m_size = glm::vec2(2 * radius);
            }
        }
    }
}