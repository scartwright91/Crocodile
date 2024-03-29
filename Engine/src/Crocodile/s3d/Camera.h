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
                enum Camera_Movement {
                    FORWARD,
                    BACKWARD,
                    LEFT,
                    RIGHT
                };

                enum ProjectionType
                {
                    ORTHOGRAPHIC,
                    PERSPECTIVE
                };

                Camera();
                ~Camera();

                void update(float dt);

                glm::mat4 getViewMatrix();
                glm::mat4 getProjectionMatrix();

                ProjectionType projectionType = PERSPECTIVE;

                // move camera
                void processMovement(Camera_Movement direction, float dt);
                void processMouseMovement(float xoffset, float yoffset);
                void processMouseScroll(float yoffset);

                // camera vectors
                glm::vec3 position = glm::vec3(0.f, 0.f, 3.f);
                glm::vec3 front = glm::vec3(0.f, 0.f, -1.f);
                glm::vec3 target = glm::vec3(0.f);
                glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
                glm::vec3 right;

                // camera parameters
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
