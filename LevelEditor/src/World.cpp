#include "World.h"

World::World(s2d::Scene *scene, ResourceManager* rm) : scene(scene), rm(rm)
{
    levels.push_back(new Level("level0", scene, rm, glm::vec2(0.f), glm::vec2(600.f)));
    levels.push_back(new Level("level1", scene, rm, glm::vec2(1000.f), glm::vec2(2000.f)));
    levels.push_back(new Level("level2", scene, rm, glm::vec2(-1000.f), glm::vec2(2000.f, 500.f)));
}

World::~World()
{
}

void World::update(float dt, glm::vec2 mousePos)
{
    worldPosition = scene->camera->getWorldfromScreenPosition(
        glm::vec2(mousePos.x, scene->window->getMouseScreenPosition().y + (2 * ImGui::GetStyle().FramePadding.y)),
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
        if (ImGui::Button("Delete"))
            deleteLevel();
        ImGui::End();
    }
}

void World::deleteLevel()
{
    levels.erase(std::remove(levels.begin(), levels.end(), selectedLevel), levels.end());
    delete selectedLevel;
    selectedLevel = nullptr;
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
