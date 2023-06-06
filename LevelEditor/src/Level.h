#pragma once

#include <string>
#include <vector>
#include <map>

#include "Crocodile/s2d/Scene.h"
#include "Crocodile/ResourceManager.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

using namespace Crocodile;

struct SceneEntityData
{
    std::string label;
    std::string layer;
    std::string texture;
    glm::vec2 pos;
    glm::vec2 size;
    float rotation;
    float alpha;
};

struct EntityData
{
    std::string label;
    glm::vec2 size;
    glm::vec3 colour;
    ResourceManager::TextureData texture;
};

struct LevelData
{
    std::string name;
    glm::vec2 canvasSize;
    glm::vec3 canvasColor;
    std::vector<s2d::Layer *> layers;
    std::vector<EntityData *> entitiesData;
    std::vector<SceneEntityData *> sceneEntityData;
    std::vector<ResourceManager::TextureData> textures;
};

class Level
{
public:
    Level(LevelData data, s2d::Scene *scene, ResourceManager *rm);
    Level(std::string name, s2d::Scene *scene, ResourceManager *rm, glm::vec2 canvasSize);

    std::string name = "";

    void update();
    void renderImGui();
    LevelData serialise();
    void scaleEdges(float v);

    s2d::Scene *scene = nullptr;
    s2d::Object *canvas = nullptr;

    std::vector<s2d::Object *> edges = {};
    std::vector<s2d::Layer *> layers = {};
    std::vector<EntityData *> entitiesData = {};
    std::vector<ResourceManager::TextureData> textures = {};

private:
    ResourceManager *rm = nullptr;

    bool placeMode = true;
    float timer = 0.f;
    s2d::Object *edgeSelected = nullptr;

    // canvas functions
    glm::vec3 canvasColour = glm::vec3(0.f);
    glm::vec3 edgeColour = glm::vec3(0.1f);
    float edgeWidth = 20.f;
    float startEdgeWidth = 20.f;
    void selectEdge(glm::vec2 mouse);
    void moveEdge(glm::vec2 mouse);
    void updateCanvas();
    void updateEdges();
    void initCanvasEdges();

    // load functions
    void loadPlacedEntities(LevelData data);

    // general ui
    glm::vec2 mouseWorldPos = glm::vec2(0.f);
    void calculateMouseWorldPos(glm::vec2 mouse);

    // imgui functions
    ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
    ImVec2 tableSize = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 8);
    void levelOptions();
    void levelInfo();
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

    // Objects
    s2d::Object *placementObject = nullptr;
    // placement
    float tmpScale = 1.f;
    float tmpAlpha = 1.f;
    float tmpRotation = 0.f;
    float placementTimer = glfwGetTime();
    const char *selectedPlacementLayer = "";
    const char *tmpPlacementEntity = "";
    bool placeMultiple = true;
    EntityData *selectedEntityData;
    void createEntityFromData(EntityData *entityData);
    void selectPlacementLayer();
    void selectPlacementObject();
    void moveObject(s2d::Object *obj, glm::vec2 mouse);
    // selection
    s2d::Object *selectedObject = nullptr;
    bool moveSelectedObject = false;
    void selectObject();
};