#pragma once

#include <glm/glm.hpp>

#include "../Object.h"

namespace Crocodile
{
    namespace s2d
    {
        namespace shapes
        {
            class CROCODILE_API Circle : public Object
            {
            public:
                Circle(float radius);

                float radius;
            };
        }
    }
}