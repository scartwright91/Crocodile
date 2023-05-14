#pragma once

#include <iostream>
#include <string>

#include "imgui.h"
#include "ImGuiFileDialog.h"

#include "Project.h"

class StartScreen
{
public:
    StartScreen();
    ~StartScreen();

    Project *getProject();
    bool isActive();
    void renderImGui();

private:
    char projectName[64] = "";
    std::string projectFolder = "";

    bool active = true;
    bool renderNewProjectOptions = false;

    std::string getProjectPath();
};
