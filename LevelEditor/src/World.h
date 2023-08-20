#pragma once

#include <vector>

#include "Crocodile/s2d/Scene.h"
#include "imgui.h"

#include "Level.h"

using namespace Crocodile;

struct Connection
{
    Level *A = nullptr;
    Level *B = nullptr;
    s2d::Object *rect = nullptr;
};

class World
{

public:
    World(s2d::Scene *scene, std::vector<Level *> levels);
    ~World();

    void update(float dt, glm::vec2 mousePos);
    void renderImGui();

    std::vector<Level *> levels = {};

    // level functionality
    Level *selectedLevel = nullptr;
    glm::vec2 worldPosition = glm::vec2(0.f);
    void selectLevel();

    // connection functionality
    Connection *selectedConnection = nullptr;
    std::vector<Connection *> connections = {};
    void selectConnection();

private:
    s2d::Scene *scene = nullptr;
};