#pragma once

#include <string>
#include <vector>

#include "Crocodile/s2d/Scene.h"
#include "Crocodile/ResourceManager.h"
#include "imgui.h"
#include "json/writer.h"
#include "json/json.h"
#include <iostream>
#include <fstream>

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

    Level *activeLevel = nullptr;
    std::vector<Level *> levels = {};

private:
    s2d::Scene *scene = nullptr;
    ResourceManager *rm = nullptr;

    s2d::Object *camera = nullptr;
    float currentZoom = 0.0f;
    float zoomTimer = 0.f;

    void zoom();
    void move();
    void load();
    void save();

    void init();

    // imgui functions
    void showImGuiMainMenu();
};