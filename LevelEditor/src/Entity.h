#pragma once

#include <vector>
#include <map>
#include "Crocodile/s2d/Object.h"

using namespace Crocodile;

class Entity
{
public:
    Entity(s2d::Object *obj, std::string layer);

    s2d::Object *obj = nullptr;
    std::string layer = "";

    std::vector<glm::vec2> movementPath = {};
    std::map<std::string, std::string> attributes = {};
};