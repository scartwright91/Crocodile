#include "Line.h"

namespace Crocodile
{
    namespace s2d
    {
        namespace shapes
        {
            Line::Line(glm::vec2 p1, glm::vec2 p2) : p1(p1), p2(p2)
            {
                renderMethod = "line";
                v = p1 - p2;
            }
        }
    }
}