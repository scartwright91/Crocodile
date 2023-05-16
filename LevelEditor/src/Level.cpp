#include "Level.h"

Level::Level(std::string name, s2d::Scene *scene) : name(name), scene(scene)
{
    canvas = new s2d::Object();
    canvas->size = glm::vec2(600.f);
    canvas->color = glm::vec3(0.8f, 0.3f, 0.1f);
    scene->addChild(canvas, "canvas");
    createCanvasEdges();
}

void Level::createCanvasEdges()
{
    // left
    s2d::Object *left = new s2d::Object();
    left->setPosition(canvas->getPosition());
    left->size = glm::vec2(20.f, 600.f);
    left->color = glm::vec3(0.f);
    scene->addChild(left, "canvas_edges");

    // right
    s2d::Object *right = new s2d::Object();
    glm::vec2 pos = canvas->getPosition();
    right->setPosition(glm::vec2(pos.x + canvas->size.x, pos.y));
    right->size = glm::vec2(20.f, 600.f);
    right->color = glm::vec3(0.f);
    scene->addChild(right, "canvas_edges");

    // top
    // s2d::Object *top = new s2d::Object();
    // left->setPosition(canvas->getPosition());
    // left->size = glm::vec2(20.f, 600.f);
    // left->color = glm::vec3(0.f);
    // scene->addChild(left, "canvas_edges");

    // // bottom
    // s2d::Object *bottom = new s2d::Object();
    // left->setPosition(canvas->getPosition());
    // left->size = glm::vec2(20.f, 600.f);
    // left->color = glm::vec3(0.f);
    // scene->addChild(left, "canvas_edges");

    edges["left"] = left;
    edges["right"] = right;
}