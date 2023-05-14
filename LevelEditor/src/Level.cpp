#include "Level.h"

Level::Level(std::string name, s2d::Scene *scene) : name(name), scene(scene)
{
    canvas = new s2d::Object();
    canvas->size = glm::vec2(600.f);
    canvas->color = glm::vec3(0.8f, 0.3f, 0.1f);
    scene->addChild(canvas, "canvas");
}