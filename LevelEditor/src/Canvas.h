#pragma once

#include "Crocodile/s2d/Scene.h"
#include "imgui.h"

#include "Grid.h"

using namespace Crocodile;

class Canvas
{
public:
    Canvas(s2d::Scene *scene, glm::vec2 pos, glm::vec2 size);
    Canvas(s2d::Scene *scene, glm::vec2 pos, glm::vec2 size, glm::vec3 color);
    ~Canvas();

    void update(float dt, glm::vec2 mouse);
    void renderImGui();

    glm::vec2 sceneMousePos = glm::vec2(0.f);
    glm::vec2 levelMousePos = glm::vec2(0.f);
    glm::vec2 levelMousePosGrid = glm::vec2(0.f);
    void calculateLevelPosition();
    glm::vec2 getLevelGridPosition();
    glm::vec2 getWorldPosition(bool snapToGrid);

    // canvas
    glm::vec2 pos = glm::vec2(0.f);
    s2d::Object *canvas = nullptr;
    glm::vec3 canvasColour = glm::vec3(0.01f, 0.41f, 0.58f);
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
    bool showGrid = false;
    int gridSizeX = 100;
    int gridSizeY = 100;
    void initGrid();

private:
    s2d::Scene *scene = nullptr;
    int round(float v);
};