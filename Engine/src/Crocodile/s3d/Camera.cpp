#include "Camera.h"
#include <iostream>

namespace Crocodile
{
    namespace s3d
    {

        CameraQuaternion::CameraQuaternion() : position(0, 0, 0), orientation(), up(0, 1, 0), forward(0, 0, -1), right(1, 0, 0)
        {
            updateCameraVectors();
        }

        void CameraQuaternion::update(float dt)
        {

        }

        void CameraQuaternion::move(Camera_Movement direction, float dt)
        {
            float velocity = speed * dt;
            if (direction == FORWARD)
                position += forward * velocity;
            if (direction == BACKWARD)
                position -= forward * velocity;
            if (direction == LEFT)
                position -= right * velocity;
            if (direction == RIGHT)
                position += right * velocity;
        }

        void CameraQuaternion::rotate(float theta, glm::vec3 axis)
        {
            Quaternion q = Quaternion::fromAxisAngle(theta, axis.x, axis.y, axis.z);
            orientation = q * orientation;
            orientation.normalize();
            updateCameraVectors();
        }

        void CameraQuaternion::updateCameraVectors()
        {
            // Rotate the forward vector
            Quaternion forwardQuat = Quaternion(0, forward.x, forward.y, forward.z);
            Quaternion result = orientation * forwardQuat * orientation.conjugate();
            forward = {result.x, result.y, result.z};
            forward = glm::normalize(forward);

            // Rotate the right vector
            Quaternion rightQuat = Quaternion(0, right.x, right.y, right.z);
            result = orientation * rightQuat * orientation.conjugate();
            right = {result.x, result.y, result.z};
            right = glm::normalize(right);

            // Rotate the up vector
            Quaternion upQuat = Quaternion(0, up.x, up.y, up.z);
            result = orientation * upQuat * orientation.conjugate();
            up = {result.x, result.y, result.z};
            up = glm::normalize(up);
        }

        CameraEuler::CameraEuler()
        {
            updateCameraVectors();
        }

        void CameraEuler::processMovement(Camera_Movement direction, float dt)
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

        void CameraEuler::processMouseMovement(float xoffset, float yoffset)
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
        
        void CameraEuler::invertPitch()
        {
            pitch = -pitch;
            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
            updateCameraVectors();
        }

        void CameraEuler::processMouseScroll(float yoffset)
        {
            zoom -= yoffset;
            if (zoom < 1.0f)
                zoom = 1.0f;
            if (zoom > 45.0f)
                zoom = 45.0f;
        }

        glm::mat4 CameraEuler::getViewMatrix()
        {
            return  glm::lookAt(position, position + front, up);
        }

        glm::mat4 CameraEuler::getProjectionMatrix()
        {
            glm::mat4 proj;
            if (projectionType == ORTHOGRAPHIC)
                proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, frustrumMin, frustrumMax);
            else if (projectionType == PERSPECTIVE)
                proj = glm::perspective(glm::radians(zoom), 1280.f / 720.f, frustrumMin, frustrumMax);
            return proj;
        }

        void CameraEuler::update(float dt)
        {
            updateCameraVectors();
        }

        void CameraEuler::updateCameraVectors()
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
