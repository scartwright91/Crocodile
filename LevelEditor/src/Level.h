#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>

#include "Crocodile/s2d/Scene.h"
#include "Crocodile/ResourceManager.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

#include "Canvas.h"
#include "EntityManager.h"

using namespace Crocodile;
namespace fs = std::filesystem;

struct LevelData
{
    // level name
    std::string name;
    // world position of canvas
    glm::vec2 canvasPos;
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
    // particle Entities placed in the scene
    std::vector<s2d::SceneParticleEntityData *> SceneParticleEntityData;
    // Textures that have been used by entities
    std::vector<ResourceManager::TextureData> textures;
};

class Level
{
public:
    Level(LevelData data, s2d::Scene *scene, ResourceManager *rm);
    Level(std::string name, s2d::Scene *scene, ResourceManager *rm, glm::vec2 canvasPos, glm::vec2 canvasSize);

    char name[64] = "";

    void load();
    void clear();
    void update(float dt, glm::vec2 mouse);
    void renderImGui();
    LevelData serialise();

    Canvas *canvas = nullptr;
    s2d::Scene *scene = nullptr;

    std::vector<s2d::Layer *> layers = {};
    std::vector<s2d::EntityData *> entitiesData = {};
    std::vector<ResourceManager::TextureData> textures = {};

    glm::vec2 sceneMousePos = glm::vec2(0.f);
    float tmpTimer = glfwGetTime();

private:
    ResourceManager *rm = nullptr;

    fs::path currentPath;

    bool updateLevel = true;
    bool placeMode = true;
    float timer = 0.f;
    s2d::Object *edgeSelected = nullptr;

    // misc stuff
    glm::vec2 viewportSize = glm::vec2(0.f);
    float titleBarHeight = 0.f;

    // load functions
    void loadPlacedEntities(LevelData data);

    // imgui functions
    ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
    ImVec2 tableSize = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 8);
    void levelOptions();
    void levelInfo();
    void sceneTree();
    void placementUI();

    // --------- Manage ----------
    // canvas

    // layers
    void addLayer();
    void removeLayer();
    void createLayersTable();
    char tmpLayerName[64] = "";
    ImVector<int> layerRowSelection = {};

    // entities
    EntityManager *entityManager = nullptr;
    void addEntity();
    void modifyEntity();
    void removeEntity();
    void selectEntityTexture();
    void selectParticleTexture();
    void createEntitiesTable();
    char tmpEntityName[64] = "";
    glm::vec3 tmpEntityColour = glm::vec3(0.f);
    int tmpWidth = 0;
    int tmpHeight = 0;
    const char *tmpNewTexture = "";
    ImVector<int> entityRowSelection = {};
    const char *selectedPlacementObjectType = "entity";
    const char *selectedPlacementLayer = "";
    const char *tmpPlacementEntity = "";
    const char *placementObjectTypes[5] = {"entity", "text", "particles", "light", "zone"};
    void selectPlacementLayer();
    void selectPlacementObject();
    void selectPlacementObjectType();

    // textures
    void addTexture();
    void createTextureTable();
    char tmpTextureName[64] = "";
    std::string tmpTexturePath = "";

    // camera
    float tmpCameraZoom = 1.f;
};