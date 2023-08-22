#pragma once

#include <iostream>
#include <string>
#include <filesystem>

#include "Crocodile/ResourceManager.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

#include "Project.h"

namespace fs = std::filesystem;

class StartScreen
{
public:
    StartScreen();
    ~StartScreen();

    Project *getProject(ResourceManager *rm);
    bool isActive();
    void renderImGui();

    std::string projectFolder = "";

private:
    fs::path currentPath;
    char projectName[64] = "";
    std::string projectNameFromFile = "";

    bool active = true;
    bool loadProjectFromFile = false;
    std::string renderOptions = "recent_projects";

    std::string getProjectName();

    void recentProjects();
    void newProject();
    void loadProject();
};
