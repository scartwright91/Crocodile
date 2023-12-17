#include "Camera.h"

namespace Crocodile
{
    namespace s3d
    {

        Camera::Camera()
        {
            updateCameraVectors();
        }

        Camera::~Camera()
        {

        }

        glm::mat4 Camera::getViewMatrix()
        {
            return  glm::lookAt(position, position + front, up);
        }

        glm::mat4 Camera::getProjectionMatrix()
        {
            return glm::perspective(glm::radians(Zoom), 1280.f / 720.f, 0.1f, 100.0f);
        }

        void Camera::update(float dt)
        {

        }

        void Camera::updateCameraVectors()
        {
            // calculate the new Front vector
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front = glm::normalize(front);
            // also re-calculate the Right and Up vector
            right = glm::normalize(glm::cross(front, up));
            up = glm::normalize(glm::cross(right, front));
        }

    }
}
