#pragma once

#include "Crocodile/s2d/Scene.h"

using namespace Crocodile;

class TextEntity
{
public:
    TextEntity(s2d::Scene *scene, s2d::Text *text, std::string layer);

    void select();
    void deselect();

    s2d::Text *text = nullptr;
    std::string layer;

private:
    s2d::Scene *scene = nullptr;
};