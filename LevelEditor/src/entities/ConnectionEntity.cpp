#include "ConnectionEntity.h"

ConnectionEntity::ConnectionEntity(
    s2d::Scene *scene,
    std::string destination,
    std::string layer,
    s2d::Object *collision) : scene(scene), destination(destination), layer(layer), collision(collision)
{
    spawn = new s2d::Object();
    spawn->setPosition(collision->getPosition());
    spawn->size = glm::vec2(50.f);
    spawn->color = glm::vec3(0.f, 0.f, 1.f);
}

ConnectionEntity::~ConnectionEntity()
{
    delete spawn;
}