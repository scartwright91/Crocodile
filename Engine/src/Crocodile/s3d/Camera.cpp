#include "Camera.h"
#include <iostream>

namespace Crocodile
{
    namespace s3d
    {

        CameraQuaternion::CameraQuaternion(int windowWidth, int windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight),
            m_position(0, 0, 0), m_orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        {
            calculateViewMatrix();
            calculateProjectionMatrix();
        }

        void CameraQuaternion::setPosition(glm::vec3 position)
        {
            m_position = position;
            calculateViewMatrix();
        }

        void CameraQuaternion::setWindowDimensions(int windowWidth, int windowHeight)
        {
            this->windowWidth = windowWidth;
            this->windowHeight = windowHeight;
        }

        void CameraQuaternion::move(Camera_Movement direction, float dt)
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

        void CameraQuaternion::rotateYaw(const float theta) {
            glm::quat q = glm::angleAxis(glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
            m_orientation = q * m_orientation;
            m_orientation = glm::normalize(m_orientation);
            calculateViewMatrix();
        }

        void CameraQuaternion::rotatePitch(const float theta) {
            glm::quat q = glm::angleAxis(glm::radians(theta), glm::vec3(1.0f, 0.0f, 0.0f));
            m_orientation = m_orientation * q;
            m_orientation = glm::normalize(m_orientation);
            calculateViewMatrix();
        }

        void CameraQuaternion::calculateViewMatrix()
        {
            glm::mat4 rotation = glm::mat4_cast(glm::conjugate(m_orientation));
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_position);
            m_view = rotation * translation;
        }

        void CameraQuaternion::calculateProjectionMatrix()
        {
            float aspectRatio = (float)windowWidth / (float)windowHeight;
            m_proj = glm::perspective(glm::radians(zoom), aspectRatio, frustrumMin, frustrumMax);
        }

    }
}
