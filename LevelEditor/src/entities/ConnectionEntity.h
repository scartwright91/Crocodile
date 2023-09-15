#pragma once

#include "Crocodile/s2d/Scene.h"

using namespace Crocodile;

struct ConnectionEntityData
{
    std::string layer;
    std::string destination;
    glm::vec2 collisionPos;
    glm::vec2 collisionWorldPos;
    glm::vec2 collisionSize;
    glm::vec2 spawnPos;
    glm::vec2 spawnWorldPos;
};

class ConnectionEntity
{
public:
    ConnectionEntity(s2d::Scene *scene, std::string destination, std::string layer, s2d::Object *collision);
    ~ConnectionEntity();

    std::string destination = "";
    std::string layer = "";
    s2d::Object *collision = nullptr;
    s2d::Object *spawn = nullptr;

private:
    s2d::Scene *scene = nullptr;
};