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

    void update(float dt, bool mouseOnImGuiWindow);
    void renderImGui();

    Project *project = nullptr;

    Level *activeLevel = nullptr;
    std::vector<Level *> levels = {};

    float fontImGuiScale = 1.f;

private:
    s2d::Scene *scene = nullptr;
    ResourceManager *rm = nullptr;

    s2d::Object *camera = nullptr;
    float speed = 300.f;
    float currentZoom = 0.0f;
    float zoomTimer = 0.f;
    float commandTimer = 0.f;

    void zoom();
    void move(float dt);
    void load();
    void save();
    void close();

    void init();

    // imgui functions
    glm::vec2 sceneMousePosition = glm::vec2(0.f);
    glm::vec2 viewportSize = glm::vec2(0.f);
    bool mouseOnSceneWindow = false;
    bool mouseOnImGuiWindow = false;
    void showImGuiMainMenu();
    bool showParticleEditor = false;
    bool showImGuiParticleEditor(bool show);
};