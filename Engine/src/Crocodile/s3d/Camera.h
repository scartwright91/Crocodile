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

                void update(float dt);

                glm::mat4 getViewMatrix();
                glm::mat4 getProjectionMatrix();

                glm::vec3 position = glm::vec3(0.f);
                glm::vec3 front = glm::vec3(0.f, 0.f, -1.f);
                glm::vec3 up = glm::vec3(0.f, 0.f, 1.f);
                glm::vec3 right;

                // Default camera values
                float Yaw         = -90.0f;
                float Pitch       =  0.0f;
                float Speed       =  2.5f;
                float Sensitivity =  0.1f;
                float Zoom        =  45.0f;

            private:
                void updateCameraVectors();

        };
    }
}
