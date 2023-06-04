#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "imgui.h"
#include "json/writer.h"
#include "json/json.h"

#include "Level.h"

class Project
{
public:
    Project(std::string name, std::string path, bool loadProjectFromFile);

    std::string name = "";
    std::string path = "";

    void save(LevelData ld);
    void load();

    void undo();
    void redo();
};