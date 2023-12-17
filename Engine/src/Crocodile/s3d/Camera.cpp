#include "Camera.h"

namespace Crocodile
{
    namespace s3d
    {

        Camera::Camera()
        {
            this->position = glm::vec3(0.f, 0.f, -10.f);
            this->front = glm::vec3(0.f, 0.f, -1.f); 
        }

        Camera::~Camera()
        {

        }

        glm::mat4 Camera::getViewMatrix()
        {
            return  glm::lookAt(position, position + front, glm::vec3(0.f, 1.f, 0.f));
        }

        glm::mat4 Camera::getProjectionMatrix()
        {
            return glm::mat4(1.f);
        }

    }
}
