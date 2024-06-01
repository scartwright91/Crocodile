#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../Core.h"
#include "Quaternion.h"


namespace Crocodile
{
    namespace s3d
    {
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

        class CROCODILE_API CameraController
        {
            public:
                CameraController(int windowWidth, int windowHeight);

                void move(Camera_Movement direction, float dt);
                // left-right rotation
                void rotateYaw(const float theta);
                // up-down rotation
                void rotatePitch(const float theta);
                void invertPitch();

                inline glm::vec3 getPosition() const { return m_position; };
                inline glm::mat4 getViewMatrix() const { return m_view; };
                inline glm::mat4 getProjectionMatrix() const { return m_proj; };
                void setPosition(glm::vec3 position);
                void setWindowDimensions(int windowWidth, int windowHeight);

            private:

                ProjectionType m_projectionType = PERSPECTIVE;

                // projection parameters
                int windowWidth;
                int windowHeight;
                float frustrumMin = 0.1f;
                float frustrumMax = 10000.f;
                float speed       =  1000.f;
                float zoom        =  45.0f;
                // view parameters
                glm::vec3 m_position;
                glm::quat m_orientation;

                // movement vectors
                inline glm::vec3 getForwardVector() const { return m_orientation * glm::vec3(0.0f, 0.0f, -1.0f); };
                inline glm::vec3 getRightVector() const { return m_orientation * glm::vec3(1.0f, 0.0f, 0.0f); };
                inline glm::vec3 getUpVector() const { return m_orientation * glm::vec3(0.0f, 1.0f, 0.0f); };
                
                // view and projection matrices
                glm::mat4 m_view;
                glm::mat4 m_proj;
                void calculateViewMatrix();
                void calculateProjectionMatrix();
            
        };

    }
}
