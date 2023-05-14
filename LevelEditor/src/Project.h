#pragma once

#include <string>

#include "imgui.h"

class Project
{
public:
    Project(std::string name, std::string path);

    std::string name = "";
    std::string path = "";

    void renderImGui();
    void save();
    void load();
};