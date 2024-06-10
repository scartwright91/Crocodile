#include "Object.h"
#include <iostream>

namespace Crocodile
{
    namespace s3d
    {

        Object::Object() : m_position(glm::vec3(0.f))
        {
        }

        Object::Object(glm::vec3 position) : m_position(position)
        {
        }

        glm::mat4 Object::calculateModelMatrix()
		{
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, m_position);
			// model = glm::rotate(model, glm::radians(0.2f), m_rotation);
            model = glm::scale(model, m_size);
			return model;
		}

    }
}
