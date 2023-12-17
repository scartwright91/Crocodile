#pragma once

#include "Object.h"
#include "../Core.h"

namespace Crocodile
{
    namespace s3d
    {
        class CROCODILE_API Camera
        {
            public:
                Camera();
                ~Camera();

                glm::mat4 getViewMatrix();
                glm::mat4 getProjectionMatrix();

                glm::vec3 position;
                glm::vec3 front;

            private:
        };
    }
}
