
#include <iostream>
#include <string>

#include "imgui.h"
#include "ImGuiFileDialog.h"

class StartScreen
{
public:
    StartScreen();
    ~StartScreen();

    void renderImGui();

private:
    char projectName[64] = "";
    std::string projectFolder = "";

    bool renderNewProjectOptions = false;

    std::string getProjectPath();
};
