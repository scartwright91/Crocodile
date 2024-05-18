#include "Camera.h"
#include <iostream>

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

        void Camera::processMovement(Camera_Movement direction, float dt)
        {
            float velocity = Speed * dt;
            if (direction == FORWARD)
                position += front * velocity;
            if (direction == BACKWARD)
                position -= front * velocity;
            if (direction == LEFT)
                position -= right * velocity;
            if (direction == RIGHT)
                position += right * velocity;
        }

        void Camera::processMouseMovement(float xoffset, float yoffset)
        {
            xoffset *= Sensitivity;
            yoffset *= Sensitivity;

            Yaw   += xoffset;
            Pitch += yoffset;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;

            // update Front, Right and Up Vectors using the updated Euler angles
            updateCameraVectors();
        }

        void Camera::processMouseScroll(float yoffset)
        {
            Zoom -= yoffset;
            if (Zoom < 1.0f)
                Zoom = 1.0f;
            if (Zoom > 45.0f)
                Zoom = 45.0f;
        }

        glm::mat4 Camera::getViewMatrix()
        {
            return  glm::lookAt(position, position + front, up);
        }

        glm::mat4 Camera::getProjectionMatrix()
        {
            glm::mat4 proj;
            if (projectionType == ORTHOGRAPHIC)
                proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, 0.1f, 1000.0f);
            else if (projectionType == PERSPECTIVE)
                proj = glm::perspective(glm::radians(Zoom), 1280.f / 720.f, 0.1f, 1000.0f);
            return proj;
        }

        void Camera::update(float dt)
        {
            updateCameraVectors();
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
