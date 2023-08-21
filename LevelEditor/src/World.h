#pragma once

#include <vector>

#include "Crocodile/ResourceManager.h"
#include "Crocodile/s2d/Scene.h"
#include "imgui.h"

#include "Level.h"
#include "Connection.h"

using namespace Crocodile;

struct WorldData
{
    unsigned int nLevels;
    std::vector<LevelData> levels;
    unsigned int nConnections;
    std::vector<ConnectionData> connections;
};

class World
{

public:
    World(s2d::Scene *scene, ResourceManager *rm);
    World(WorldData wd, s2d::Scene *scene, ResourceManager *rm);
    ~World();

    WorldData serialise();
    void update(float dt, glm::vec2 mousePos);
    void renderImGui();

    std::vector<const char *> getLevelNames();
    Level *getLevel(std::string name);

    std::vector<Level *> levels = {};
    std::vector<Connection *> connections = {};

    bool mouseOnImGuiWindow = false;

    // level functionality
    Level *activeLevel = nullptr;
    Level *selectedLevel = nullptr;
    Level *tmpLevel = nullptr;
    glm::vec2 worldPosition = glm::vec2(0.f);
    bool movingLevel = false;
    char tmpLevelName[64] = "";
    int tmpLevelWidth = 1000;
    int tmpLevelHeight = 1000;
    void enterLevel();
    void deleteLevel();
    void selectLevel();
    void createLevel();
    void placeLevel();
    void moveLevel();

    // connection functionality
    void updateAllConnections();
    Connection *selectedConnection = nullptr;
    Connection *tmpConnection = nullptr;
    bool movingConnection = false;
    const char *selectedConnectionA = "";
    const char *selectedConnectionB = "";
    int tmpConnectionWidth = 100;
    void createConnection();
    void selectConnectionA();
    void selectConnectionB();
    void selectConnection();
    void updateConnection();
    void deleteConnection();
    void moveConnection();

private:
    ResourceManager *rm = nullptr;
    s2d::Scene *scene = nullptr;
};