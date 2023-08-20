#include "World.h"

World::World(s2d::Scene *scene, ResourceManager *rm) : scene(scene), rm(rm)
{
    levels.push_back(new Level("level0", scene, rm, glm::vec2(0.f), glm::vec2(600.f)));
    levels.push_back(new Level("level1", scene, rm, glm::vec2(1000.f), glm::vec2(2000.f)));
    levels.push_back(new Level("level2", scene, rm, glm::vec2(-1000.f), glm::vec2(2000.f, 500.f)));
    for (Level *level : levels)
        level->canvas->initCanvasEdges();
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

    // level selection / deselection
    if (selectedLevel == NULL)
        selectLevel();
    else
    {
        if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_2))
            selectedLevel = nullptr;
    }

    // creating new level
    if (tmpLevel != NULL)
    {
        placeLevel();
    }
}

void World::renderImGui()
{
    ImGui::Begin("Manage");
    ImGui::Text("Some world options...");
    if (ImGui::CollapsingHeader("New level"))
    {
        ImGui::InputText("Entity name", tmpLevelName, 64);
        if (tmpLevelWidth <= 100)
            tmpLevelWidth = 100;
        if (tmpLevelHeight <= 100)
            tmpLevelHeight = 100;
        ImGui::InputInt("Width", &tmpLevelWidth);
        ImGui::InputInt("Height", &tmpLevelHeight);
        if (ImGui::Button("Add"))
            createLevel();
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
        ImGui::Begin("Level options");
        ImGui::Text(selectedLevel->name);
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

void World::createLevel()
{
    tmpLevel = new Level(std::string(tmpLevelName), scene, rm, glm::vec2(0.f), glm::vec2(tmpLevelWidth, tmpLevelHeight));
}

void World::placeLevel()
{
    // move level
    tmpLevel->canvas->canvas->setPosition(worldPosition);
    if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
    {
        levels.push_back(tmpLevel);
        tmpLevel->canvas->initCanvasEdges();
        tmpLevel = nullptr;
    }
    else if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_2))
    {
        delete tmpLevel;
        tmpLevel = nullptr;
    }
}