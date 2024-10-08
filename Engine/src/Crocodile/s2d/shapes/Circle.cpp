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
                setSize(glm::vec2(2 * radius));
            }
        }
    }
}