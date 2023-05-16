#pragma once

#include <string>
#include <map>

#include "Crocodile/s2d/Scene.h"

using namespace Crocodile;

class Level
{
public:
    Level(std::string name, s2d::Scene *scene);

    std::string name = "";

    s2d::Scene *scene = nullptr;
    s2d::Object *canvas = nullptr;

    std::map<std::string, s2d::Object *> edges = {};

private:
    void createCanvasEdges();
};