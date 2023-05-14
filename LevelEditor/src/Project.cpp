#include "Project.h"

Project::Project(std::string name, std::string path)
{
    this->name = name;
    this->path = path;
}

void Project::renderImGui()
{
    ImGui::ShowDemoWindow();
}

void Project::save()
{
}

void Project::load()
{
}
