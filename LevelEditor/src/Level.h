#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>

#include "Crocodile/s2d/Scene.h"
#include "Crocodile/ResourceManager.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

#include "Entity.h"
#include "TextEntity.h"

using namespace Crocodile;
namespace fs = std::filesystem;

struct LevelData
{
    // level name
    std::string name;
    // world size of canvas
    glm::vec2 canvasSize;
    // background colour of canvas
    glm::vec3 canvasColor;
    // layer information
    std::vector<s2d::Layer *> layers;
    // Unique entity types to place in the scene
    std::vector<s2d::EntityData *> entitiesData;
    // Entities that have been placed in the scene
    std::vector<s2d::SceneEntityData *> sceneEntityData;
    // Text Entities that have been placed in the scene
    std::vector<s2d::SceneTextEntityData *> sceneTextEntityData;
    // Textures that have been used by entities
    std::vector<ResourceManager::TextureData> textures;
};

class Level
{
public:
    Level(LevelData data, s2d::Scene *scene, ResourceManager *rm);
    Level(std::string name, s2d::Scene *scene, ResourceManager *rm, glm::vec2 canvasSize);

    char name[64] = "";

    void update(glm::vec2 mouse);
    void renderImGui();
    LevelData serialise();
    void scaleEdges();

    s2d::Scene *scene = nullptr;
    s2d::Object *canvas = nullptr;

    std::vector<s2d::Object *> edges = {};
    std::vector<s2d::Layer *> layers = {};
    std::vector<s2d::EntityData *> entitiesData = {};
    std::vector<ResourceManager::TextureData> textures = {};

    glm::vec2 sceneMousePos = glm::vec2(0.f);
    float tmpTimer = glfwGetTime();

private:
    ResourceManager *rm = nullptr;

    fs::path currentPath;

    bool placeMode = true;
    float timer = 0.f;
    s2d::Object *edgeSelected = nullptr;

    // misc stuff
    glm::vec2 viewportSize = glm::vec2(0.f);
    float titleBarHeight = 0.f;

    // canvas functions
    glm::vec3 canvasColour = glm::vec3(0.f);
    glm::vec3 edgeColour = glm::vec3(0.1f);
    float edgeWidth = 20.f;
    float startEdgeWidth = 20.f;
    void selectEdge();
    void moveEdge();
    void updateCanvas();
    void updateEdges();
    void initCanvasEdges();

    // load functions
    void loadPlacedEntities(LevelData data);

    // general ui
    glm::vec2 mouseWorldPos = glm::vec2(0.f);
    void calculateMouseWorldPos();

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

    // camera
    float tmpCameraZoom = 1.f;

    // placement
    std::vector<Entity *> placedEntities = {};
    std::vector<TextEntity *> placedTextEntities = {};
    // entity inputs
    float tmpScale = 1.f;
    float tmpAlpha = 1.f;
    float tmpRotation = 0.f;
    // text inputs
    char tmpText[64] = "";
    glm::vec3 tmpTextColor = glm::vec3(1.f);
    float tmpTextScale = 1.f;
    // particle inputs
    int tmpParticleAmount = 10;
    glm::vec3 tmpParticleColor = glm::vec3(1.f);
    float tmpParticleDirection = 0.f;
    float tmpParticleDispersion = 0.1f;
    float tmpParticleScale = 1.f;
    float tmpParticleVelocity = 1.f;
    //
    float placementTimer = glfwGetTime();
    const char *selectedPlacementType = "entity";
    const char *selectedPlacementObjectType = "entity";
    const char *selectedPlacementLayer = "";
    const char *tmpPlacementEntity = "";
    bool placeMultiple = true;
    s2d::EntityData *selectedEntityData;
    void createEntityFromData(s2d::EntityData *entityData);
    void createTextEntity();
    const char *placementTypes[3] = {"entity", "camera", "mask"};
    const char *placementObjectTypes[3] = {"entity", "text", "particles"};
    void selectPlacementType();
    void selectPlacementLayer();
    void selectPlacementObject();
    void selectPlacementObjectType();
    void deleteObject(s2d::Object *obj);
    void moveObject(s2d::Object *obj);
    // selection
    s2d::Object *selectedObject = nullptr;
    bool moveSelectedObject = false;
    void selectObject();
    float objectPathTimer = 0.f;
    std::vector<s2d::Object *> objectPath = {};
    void createObjectPath();
};