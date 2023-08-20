#pragma once

#include <vector>

#include "Crocodile/ResourceManager.h"
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
    World(s2d::Scene *scene, ResourceManager *rm);
    ~World();

    void update(float dt, glm::vec2 mousePos);
    void renderImGui();

    std::vector<Level *> levels = {};

    // level functionality
    Level *activeLevel = nullptr;
    Level *selectedLevel = nullptr;
    Level *tmpLevel = nullptr;
    glm::vec2 worldPosition = glm::vec2(0.f);
    char tmpLevelName[64] = "";
    int tmpLevelWidth = 1000;
    int tmpLevelHeight = 1000;
    void enterLevel();
    void deleteLevel();
    void selectLevel();
    void createLevel();
    void placeLevel();

    // connection functionality
    Connection *selectedConnection = nullptr;
    std::vector<Connection *> connections = {};
    void selectConnection();

private:
    ResourceManager *rm = nullptr;
    s2d::Scene *scene = nullptr;
};