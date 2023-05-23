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
    std::vector<s2d::Object *> entities = {};
    std::map<std::string, s2d::Object *> entitiesMap = {};
    std::vector<ResourceManager::TextureData> textures = {};

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
    ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
    ImVec2 tableSize = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 8);
    void levelOptions();
    void sceneTree();
    void placementUI();

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
    void selectEntityTexture();
    void createEntitiesTable();
    char tmpEntityName[64] = "";
    glm::vec3 tmpEntityColour = glm::vec3(0.f);
    int tmpWidth = 0;
    int tmpHeight = 0;
    const char *tmpNewTexture = "";
    ImVector<int> entityRowSelection = {};

    // textures
    void addTexture();
    void createTextureTable();
    char tmpTextureName[64] = "";
    ImVector<char *> getTextureNames();
    std::string tmpTexturePath = "";

    // placement
    bool placeMode = true;
    float placementTimer = glfwGetTime();
    const char *tmpPlacementLayer = "";
    const char *tmpPlacementEntity = "";
    bool placeMultiple = true;
    void selectPlacementLayer();
    void selectPlacementObject();
};