#include "Object.h"

namespace Crocodile
{
    namespace s3d
    {

        Object::Object(glm::vec3 position, glm::vec3 size) : position(position), size(size)
        {

        }

        Object::~Object()
        {

        }

        glm::mat4 Object::calculateModelMatrix()
		{
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, position);
			// model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			return model;
		}

    }
}
