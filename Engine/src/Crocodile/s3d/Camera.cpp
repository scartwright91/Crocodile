#include "Camera.h"
#include <iostream>

namespace Crocodile
{
    namespace s3d
    {

        CameraController::CameraController(int windowWidth, int windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight),
            m_position(0, 0, 0), m_orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        {
            calculateViewMatrix();
            calculateProjectionMatrix();
        }

        void CameraController::setPosition(glm::vec3 position)
        {
            m_position = position;
            calculateViewMatrix();
        }

        void CameraController::setWindowDimensions(int windowWidth, int windowHeight)
        {
            this->windowWidth = windowWidth;
            this->windowHeight = windowHeight;
        }

        void CameraController::move(Camera_Movement direction, float dt)
        {
            float velocity = speed * dt;
            if (direction == FORWARD)
                m_position += glm::normalize(getForwardVector()) * velocity;
            if (direction == BACKWARD)
                m_position -= glm::normalize(getForwardVector()) * velocity;
            if (direction == LEFT)
                m_position -= glm::normalize(getRightVector()) * velocity;
            if (direction == RIGHT)
                m_position += glm::normalize(getRightVector()) * velocity;

            calculateViewMatrix();
        }

        void CameraController::rotateYaw(const float theta) {
            glm::quat q = glm::angleAxis(glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
            m_orientation = q * m_orientation;
            m_orientation = glm::normalize(m_orientation);
            calculateViewMatrix();
        }

        void CameraController::rotatePitch(const float theta) {
            glm::quat q = glm::angleAxis(glm::radians(theta), glm::vec3(1.0f, 0.0f, 0.0f));
            m_orientation = m_orientation * q;
            m_orientation = glm::normalize(m_orientation);
            calculateViewMatrix();
        }

        void CameraController::invertPitch()
        {
            // Get the current Euler angles
            glm::vec3 eulerAngles = glm::eulerAngles(m_orientation);
            // Invert the pitch angle (X-axis rotation)
            float invertedPitch = -eulerAngles.x;
            // Create a new quaternion with the inverted pitch
            glm::quat pitchQuat = glm::angleAxis(invertedPitch, glm::vec3(1.0f, 0.0f, 0.0f));
            // Keep the yaw (Y-axis rotation) and roll (Z-axis rotation) from the original orientation
            glm::quat yawQuat = glm::angleAxis(eulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::quat rollQuat = glm::angleAxis(eulerAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));
            // Combine the quaternions
            m_orientation = yawQuat * pitchQuat * rollQuat;
            m_orientation = glm::normalize(m_orientation);
        }

        void CameraController::calculateViewMatrix()
        {
            glm::mat4 rotation = glm::mat4_cast(glm::conjugate(m_orientation));
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_position);
            m_view = rotation * translation;
        }

        void CameraController::calculateProjectionMatrix()
        {
            float aspectRatio = (float)windowWidth / (float)windowHeight;
            m_proj = glm::perspective(glm::radians(zoom), aspectRatio, frustrumMin, frustrumMax);
        }

    }
}
