#include "Entity.h"

namespace Crocodile
{
    namespace s2d
    {
        Entity::Entity(Scene *scene, std::string layer, ResourceManager *rm, SoundManager *sm) : scene(scene), layer(layer), rm(rm), sm(sm)
        {
            sprite = new Object();
        }

        Entity::~Entity()
        {
            delete sprite;
        }

        void Entity::add()
        {
            scene->addObject(sprite, layer);
        }

        void Entity::remove()
        {
            scene->removeObject(sprite, layer);
        }
    }
}