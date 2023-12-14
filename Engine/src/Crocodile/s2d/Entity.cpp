#include "Entity.h"

namespace Crocodile
{
    namespace s2d
    {
        Entity::Entity(s2d::Scene* scene, ResourceManager* rm) : scene(scene), rm(rm)
        {
            sprite = new Object();
        }

        Entity::~Entity()
        {
            delete sprite;
        }

    }
}