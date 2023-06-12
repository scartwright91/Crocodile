#include "Entity.h"

Entity::Entity(s2d::Object *obj, std::string layer) : obj(obj), layer(layer)
{
    movementPath.push_back(obj->getPosition());
}