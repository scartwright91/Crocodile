#pragma once

#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "Crocodile/s2d/Object.h"

class LevelOptions
{
public:
    LevelOptions();

    void renderImGui();

private:
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
};