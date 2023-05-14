#include "Editor.h"

Editor::Editor(
    Project *project,
    s2d::Scene *scene,
    ResourceManager *rm) : project(project), scene(scene), rm(rm)
{
    scene->layerStack->addLayer(new s2d::Layer("canvas"));
}

void Editor::renderImGui()
{
    ImGui::ShowDemoWindow();
}