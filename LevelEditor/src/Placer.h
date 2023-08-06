#pragma once

#include "Crocodile/s2d/Scene.h"

using namespace Crocodile;

class Placer
{
public:
    Placer(s2d::Scene *scene);
    ~Placer();

    void update(float dt);

private:
    s2d::Scene *scene = nullptr;
};