#pragma once

#include "Crocodile/s2d/Scene.h"

using namespace Crocodile;

class ParticleEntity
{
public:
    ParticleEntity(s2d::Scene *scene, s2d::ParticleGenerator *pg, std::string layer);

    void update();

    s2d::ParticleGenerator *pg = nullptr;
    s2d::Text *label = nullptr;
    std::string layer = "";

private:
    s2d::Scene *scene = nullptr;
};