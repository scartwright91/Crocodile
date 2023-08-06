#pragma once

#include <vector>
#include <map>
#include "Crocodile/s2d/Scene.h"

using namespace Crocodile;

class Entity
{
public:
    Entity(s2d::Scene *scene, s2d::Object *obj, std::string layer);

    bool selected = false;
    void select();
    void deselect();
    void addMovementPathPos(glm::vec2 pos);

    s2d::Object *obj = nullptr;
    s2d::Text *label = nullptr;
    std::string layer = "";

    std::vector<glm::vec2> movementPath = {};
    std::map<std::string, std::string> attributes = {};

private:
    s2d::Scene *scene = nullptr;
    std::vector<s2d::shapes::Line *> tmpLines = {};
    std::vector<s2d::Object *> tmpMarkers = {};
};