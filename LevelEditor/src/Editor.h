#pragma once

#include <string>
#include <vector>

#include "Crocodile/s2d/Scene.h"
#include "Crocodile/ResourceManager.h"
#include "imgui.h"

#include "Level.h"
#include "Project.h"

using namespace Crocodile;

class Editor
{
public:
    Editor(Project *project, s2d::Scene *scene, ResourceManager *rm);

    void update();
    void renderImGui();

    Project *project = nullptr;

    std::vector<Level *> levels = {};

private:
    s2d::Scene *scene = nullptr;
    ResourceManager *rm = nullptr;

    s2d::Object *camera = nullptr;
    float currentZoom = 0.0f;
    float zoomTimer = 0.f;

    void zoom();
    void move();

    void init();
};