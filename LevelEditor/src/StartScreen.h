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

private:
    fs::path currentPath;
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
