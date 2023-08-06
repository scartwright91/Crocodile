#pragma once

#include "Crocodile/s2d/Scene.h"
#include "imgui.h"

#include "Grid.h"

using namespace Crocodile;

class Canvas
{
public:
    Canvas(s2d::Scene *scene);
    ~Canvas();

    void update(float dt, glm::vec2 mouse);
    void renderImGui();

    glm::vec2 sceneMousePos = glm::vec2(0.f);

    // canvas
    s2d::Object *canvas = nullptr;
    glm::vec3 canvasColour = glm::vec3(0.f);
    void updateCanvas();
    void initCanvasEdges();

    // edges
    std::vector<s2d::Object *> edges = {};
    s2d::Object *edgeSelected = nullptr;
    glm::vec3 edgeColour = glm::vec3(0.1f);
    float edgeWidth = 20.f;
    float startEdgeWidth = 20.f;
    void selectEdge();
    void moveEdge();
    void scaleEdges();
    void updateEdges();

    // grid
    Grid *grid = nullptr;
    bool showGrid = true;
    int gridSizeX = 100;
    int gridSizeY = 100;
    void initGrid();

private:
    s2d::Scene *scene = nullptr;
};