#pragma once

#include <string>

#include "Crocodile/s2d/Scene.h"
#include "Crocodile/ResourceManager.h"
#include "Project.h"
#include "imgui.h"

using namespace Crocodile;

class Editor
{
public:
    Editor(Project *project, s2d::Scene *scene, ResourceManager *rm);

    void renderImGui();

    Project *project = nullptr;

private:
    s2d::Scene *scene = nullptr;
    ResourceManager *rm = nullptr;
};