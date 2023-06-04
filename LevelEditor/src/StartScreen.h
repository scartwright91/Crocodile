#pragma once

#include <iostream>
#include <string>

#include "Crocodile/ResourceManager.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

#include "Project.h"

class StartScreen
{
public:
    StartScreen();
    ~StartScreen();

    Project *getProject(ResourceManager* rm);
    bool isActive();
    void renderImGui();

private:
    char projectName[64] = "";
    std::string projectNameFromFile = "";
    std::string projectFolder = "";

    bool active = true;
    bool loadProjectFromFile = false;
    std::string renderOptions = "recent_projects";

    std::string getProjectPath();

    void recentProjects();
    void newProject();
    void loadProject();
};
