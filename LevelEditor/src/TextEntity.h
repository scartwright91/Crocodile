#pragma once

#include "Crocodile/s2d/Scene.h"

using namespace Crocodile;

class TextEntity
{
public:
    TextEntity(s2d::Scene *scene, s2d::Text *text);

private:
    s2d::Scene *scene = nullptr;
    s2d::Text *text = nullptr;
};