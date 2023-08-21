#pragma once

#include "Crocodile/s2d/Scene.h"

using namespace Crocodile;

class Connection
{
public:
    Connection(
        s2d::Scene *scene,
        float width,
        s2d::Object *a,
        s2d::Object *b);
    ~Connection();

    void update(glm::vec2 pos);

    s2d::Object *a = nullptr;
    s2d::Object *b = nullptr;
    glm::vec2 p1 = glm::vec2(0.f);
    glm::vec2 p2 = glm::vec2(0.f);
    float width = 0.f;
    std::string orientation = "";
    s2d::Object *rect = nullptr;

private:
    s2d::Scene *scene = nullptr;
    void init();
    void createRect();
};