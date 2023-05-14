#pragma once

#include <string>

#include "Crocodile/s2d/Scene.h"

using namespace Crocodile;

class Level
{
public:
    Level(std::string name, s2d::Scene *scene);

    std::string name = "";

    s2d::Scene *scene = nullptr;
    s2d::Object *canvas = nullptr;
};