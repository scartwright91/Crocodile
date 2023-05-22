#pragma once

#include <string>
#include <vector>
#include <map>

#include "Crocodile/s2d/Scene.h"
#include "Crocodile/ResourceManager.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

using namespace Crocodile;

class Level
{
public:
    Level(std::string name, s2d::Scene *scene, ResourceManager *rm, glm::vec2 canvasSize);

    std::string name = "";

    void update();
    void renderImGui();

    s2d::Scene *scene = nullptr;
    s2d::Object *canvas = nullptr;

    std::vector<s2d::Object *> edges = {};
    std::vector<s2d::Layer *> layers = {};
    std::map<std::string, s2d::Object *> entities = {};

private:
    ResourceManager *rm = nullptr;

    float timer = 0.f;
    s2d::Object *edgeSelected = nullptr;

    // canvas functions
    float edgeWidth = 20.f;
    void selectEdge();
    void moveEdge(glm::vec2 mouse);
    void updateCanvas();
    void updateEdges();
    void initCanvasEdges();

    // imgui functions
    void levelOptions();

    // layers
    void addLayer();
    void removeLayer();
    void createLayersTable();
    char tmpLayerName[64] = "";
    ImVector<int> layerRowSelection = {};

    // entities
    void addEntity();
    void modifyEntity();
    void removeEntity();
    void createEntitiesTable();
    char tmpEntityName[64] = "";
    glm::vec3 tmpEntityColour = glm::vec3(0.f);
    int tmpWidth = 0;
    int tmpHeight = 0;
    int tmpIntTexture = 0;
    ImVector<int> entityIndexRowSelection = {};
    ImVector<std::string> entityRowSelection = {};

    // resources
    void addResource();
    char tmpTextureName[64] = "";
    std::string tmpTexturePath = "";
};