#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "Crocodile/ResourceManager.h"
#include "imgui.h"
#include "json/writer.h"
#include "json/json.h"

#include "World.h"

class Project
{
public:
    Project(std::string name, std::string path, bool loadProjectFromFile, ResourceManager *rm);

    std::string name = "";
    std::string path = "";
    bool loadProjectFromFile = false;

    WorldData data;

    void save(WorldData wd);
    WorldData load();

    void undo();
    void redo();

private:
    ResourceManager *rm = nullptr;
};