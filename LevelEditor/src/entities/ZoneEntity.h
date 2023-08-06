#pragma once

#include "Crocodile/s2d/Scene.h"

using namespace Crocodile;

class ZoneEntity
{
public:
    ZoneEntity(s2d::Scene *scene);
    ~ZoneEntity();

    s2d::Object *zone = nullptr;
    glm::vec3 color = glm::vec3(0.f);

private:
    s2d::Scene *scene = nullptr;
};