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
                Object();
                Object(glm::vec3 position);


                glm::vec3 m_position;
                glm::vec3 m_size = glm::vec3(1.f);
                glm::vec3 m_rotation = glm::vec3(0.f);
                glm::vec3 m_colour = glm::vec3(1.f, 0.f, 0.f);

                glm::mat4 calculateModelMatrix();
            private:

        };
    }
}
