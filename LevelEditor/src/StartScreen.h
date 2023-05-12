
#include <iostream>
#include <string>

#include "imgui.h"

class StartScreen
{
public:
    StartScreen();
    ~StartScreen();

    void renderImGui();

private:
    std::string projectName{""};

    bool renderNewProjectOptions = false;
};
