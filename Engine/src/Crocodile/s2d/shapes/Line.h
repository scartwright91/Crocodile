#pragma once

#include <glm/glm.hpp>

#include "../Object.h"

namespace Crocodile
{
    namespace s2d
    {
        namespace shapes
        {
            class CROCODILE_API Line : public Object
            {
            public:
                Line(glm::vec2 p1, glm::vec2 p2);

                glm::vec2 v = glm::vec2(0.f);
                glm::vec2 p1 = glm::vec2(0.f);
                glm::vec2 p2 = glm::vec2(0.f);
            };
        }
    }
}