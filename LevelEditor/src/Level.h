#pragma once

#include <string>
#include <vector>

#include "Crocodile/s2d/Scene.h"
#include "imgui.h"

using namespace Crocodile;

class Level
{
public:
    Level(std::string name, s2d::Scene *scene, glm::vec2 canvasSize);

    std::string name = "";

    void update();
    void renderImGui();

    s2d::Scene *scene = nullptr;
    s2d::Object *canvas = nullptr;

    std::vector<s2d::Object *> edges = {};
    std::vector<s2d::Layer *> layers = {};

private:
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
    void showImGuiLayers();
    void createLayersTable();
    void addLayer();
    char tmpLayerName[64] = "";
    ImVector<int> rowSelection = {};
};