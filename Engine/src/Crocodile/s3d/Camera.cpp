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
            float velocity = speed * dt;
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
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw   += xoffset;
            pitch += yoffset;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            // update Front, Right and Up Vectors using the updated Euler angles
            updateCameraVectors();
        }
        
        void Camera::invertPitch()
        {
            pitch = -pitch;
            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
            updateCameraVectors();
        }

        void Camera::processMouseScroll(float yoffset)
        {
            zoom -= yoffset;
            if (zoom < 1.0f)
                zoom = 1.0f;
            if (zoom > 45.0f)
                zoom = 45.0f;
        }

        glm::mat4 Camera::getViewMatrix()
        {
            return  glm::lookAt(position, position + front, up);
        }

        glm::mat4 Camera::getProjectionMatrix()
        {
            glm::mat4 proj;
            if (projectionType == ORTHOGRAPHIC)
                proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, frustrumMin, frustrumMax);
            else if (projectionType == PERSPECTIVE)
                proj = glm::perspective(glm::radians(zoom), 1280.f / 720.f, frustrumMin, frustrumMax);
            return proj;
        }

        void Camera::update(float dt)
        {
            updateCameraVectors();
        }

        void Camera::updateCameraVectors()
        {
            // calculate the new Front vector
            glm::vec3 f;
            f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            f.y = sin(glm::radians(pitch));
            f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            front = glm::normalize(f);
            // also re-calculate the Right and Up vector
            right = glm::normalize(glm::cross(front, up));
            up = glm::normalize(glm::cross(right, front));
        }

    }
}
