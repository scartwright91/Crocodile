#include "World.h"

World::World(s2d::Scene *scene, std::vector<Level *> levels) : scene(scene), levels(levels)
{
}

World::~World()
{
}

void World::update(float dt)
{
    worldPosition = scene->camera->getWorldfromScreenPosition(
        scene->window->getMouseScreenPosition(),
        scene->windowWidth,
        scene->windowHeight);
    if (selectedLevel == NULL)
    {
        selectLevel();
        if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_2))
            selectedLevel = nullptr;
    }
}

void World::renderImGui()
{
    ImGui::Begin("Manage");
    ImGui::Text("Some world options...");
    if (ImGui::CollapsingHeader("New level"))
    {
        ImGui::Text("Level name...");
        ImGui::Text("Level size...");
        ImGui::Button("Add");
    }
    if (ImGui::CollapsingHeader("New connection"))
    {
        ImGui::Text("A:");
        ImGui::Text("B:");
        ImGui::Button("Add");
    }
    ImGui::End();
    if (selectedLevel != NULL)
    {
        ImGui::Begin(selectedLevel->name);
        ImGui::Text("Some level options...");
        ImGui::Text("Rename...");
        ImGui::Text("Move...");
        ImGui::Button("Delete");
        ImGui::End();
    }
}

void World::selectLevel()
{
    for (Level *level : levels)
    {
        if (level->canvas->canvas->getBoundingBox().intersectsPoint(worldPosition))
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
            if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
            {
                selectedLevel = level;
                break;
            }
        }
    }
}
