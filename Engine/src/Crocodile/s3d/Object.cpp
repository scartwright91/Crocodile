#include "Object.h"
#include <iostream>

namespace Crocodile
{
    namespace s3d
    {

        Object::Object(glm::vec3 position) : position(position), size(size)
        {

        }

        Object::~Object()
        {

        }

        glm::mat4 Object::calculateModelMatrix()
		{
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, position);
			model = glm::rotate(model, glm::radians(0.2f), glm::vec3(1.0f, 0.3f, 0.5f));
			return model;
		}

    }
}
