#pragma once

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core.h"

namespace Crocodile
{
    namespace s3d
    {
        class CROCODILE_API Object
        {
            public:
                Object(glm::vec3 position, glm::vec3 size);
                ~Object();

                glm::vec3 position;
                glm::vec3 size;

                glm::mat4 calculateModelMatrix();
            private:

        };
    }
}
