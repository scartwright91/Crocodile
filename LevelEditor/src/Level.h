#pragma once

#include <string>
#include <vector>
#include <map>

#include "Crocodile/s2d/Scene.h"
#include "Crocodile/ResourceManager.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

using namespace Crocodile;

struct EntityData
{
    std::string label;
    glm::vec2 size;
    glm::vec3 colour;
    ResourceManager::TextureData texture;
};

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
    std::vector<EntityData> entitiesData = {};
    std::vector<s2d::Object *> placedEntities = {};
    std::map<std::string, EntityData> entitiesDataMap = {};
    std::vector<ResourceManager::TextureData> textures = {};

private:
    ResourceManager *rm = nullptr;

    float timer = 0.f;
    s2d::Object *edgeSelected = nullptr;

    // canvas functions
    glm::vec3 canvasColour = glm::vec3(0.f);
    glm::vec3 edgeColour = glm::vec3(0.1f);
    float edgeWidth = 20.f;
    void scaleEdge(float v);
    void selectEdge(glm::vec2 mouse);
    void moveEdge(glm::vec2 mouse);
    void updateCanvas();
    void updateEdges();
    void initCanvasEdges();

    // general ui
    glm::vec2 mouseWorldPos = glm::vec2(0.f);
    s2d::Text *mouseWorldPosText = nullptr;
    void calculateMouseWorldPos(glm::vec2 mouse);

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
    std::string tmpTexturePath = "";

    // placement
    s2d::Object *placementObject = nullptr;
    float tmpScale = 1.f;
    float tmpAlpha = 1.f;
    float tmpRotation = 0.f;
    bool placeMode = true;
    float placementTimer = glfwGetTime();
    const char *selectedPlacementLayer = "";
    const char *tmpPlacementEntity = "";
    bool placeMultiple = true;
    EntityData selectedEntityData;
    void createEntityFromData(EntityData entityData);
    void selectPlacementLayer();
    void selectPlacementObject();
    void movePlacementObject(glm::vec2 mouse);
};