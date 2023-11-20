#include "Entity.h"

namespace Crocodile
{
    namespace s2d
    {
        Entity::Entity(std::string name) : name(name)
        {
            sprite = new Object();
        }

        Entity::~Entity()
        {
            delete sprite;
        }

    }
}