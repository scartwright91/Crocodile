#include "World.h"

World::World(s2d::Scene *scene, ResourceManager *rm) : scene(scene), rm(rm)
{
    levels.push_back(new Level("level0", scene, rm, glm::vec2(0.f), glm::vec2(600.f)));
    levels.push_back(new Level("level1", scene, rm, glm::vec2(1000.f), glm::vec2(2000.f)));
    levels.push_back(new Level("level2", scene, rm, glm::vec2(-1000.f), glm::vec2(2000.f, 500.f)));
    for (Level *level : levels)
    {
        level->canvas->initCanvasEdges();
        level->createLevelName();
    }
}

World::World(WorldData wd, s2d::Scene *scene, ResourceManager *rm) : scene(scene), rm(rm)
{
    for (LevelData ld : wd.levels)
    {
        Level *level = new Level(ld, scene, rm);
        level->canvas->initCanvasEdges();
        level->createLevelName();
        levels.push_back(level);
    }
}

World::~World()
{
}

WorldData World::serialise()
{
    WorldData wd = {};
    wd.nLevels = levels.size();
    wd.nConnections = connections.size();
    std::vector<LevelData> lds = {};
    for (Level *level : levels)
        lds.push_back(level->serialise());
    wd.levels = lds;
    return wd;
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
        if (movingLevel)
            moveLevel();
        if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_2))
            selectedLevel = nullptr;
    }

    // creating new level
    if (tmpLevel != NULL)
    {
        placeLevel();
    }

    if (tmpConnection != NULL)
    {
        updateConnection();
    }
}

void World::renderImGui()
{
    ImGui::Begin("Manage");
    ImGui::Text("Some world options...");
    if (ImGui::CollapsingHeader("New level"))
    {
        ImGui::InputText("Level name", tmpLevelName, 64);
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
        selectConnectionA();
        selectConnectionB();
        ImGui::InputInt("Width", &tmpConnectionWidth);
        if (ImGui::Button("Add"))
            createConnection();
    }
    ImGui::End();
    if (selectedLevel != NULL)
    {
        ImGui::Begin("Level options");
        ImGui::Text(selectedLevel->name);
        if (ImGui::InputText("Level name", selectedLevel->name, 64))
            selectedLevel->createLevelName();
        if (ImGui::Button("Move level") || scene->window->isKeyPressed(GLFW_KEY_M))
            movingLevel = true;
        if (ImGui::Button("Enter level") || scene->window->isKeyPressed(GLFW_KEY_ENTER))
            enterLevel();
        if (ImGui::Button("Delete"))
            deleteLevel();
        ImGui::End();
    }
}

std::vector<const char *> World::getLevelNames()
{
    std::vector<const char *> names = {};
    for (Level *level : levels)
        names.push_back(level->name);
    return names;
}

Level *World::getLevel(const char *name)
{
    for (Level *level : levels)
        if (level->name == name)
            return level;
    return nullptr;
}

void World::enterLevel()
{
    activeLevel = selectedLevel;
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
    tmpLevel->canvas->canvas->setPosition(worldPosition);
    if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
    {
        levels.push_back(tmpLevel);
        tmpLevel->canvas->initCanvasEdges();
        tmpLevel->createLevelName();
        tmpLevel = nullptr;
    }
    else if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_2))
    {
        delete tmpLevel;
        tmpLevel = nullptr;
    }
}

void World::moveLevel()
{
    selectedLevel->canvas->canvas->setPosition(worldPosition);
    if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
    {
        selectedLevel->canvas->initCanvasEdges();
        selectedLevel->createLevelName();
        movingLevel = false;
    }
}

void World::createConnection()
{
    s2d::Object *a = getLevel(selectedConnectionA)->canvas->canvas;
    s2d::Object *b = getLevel(selectedConnectionB)->canvas->canvas;
    tmpConnection = new Connection(
        scene,
        tmpConnectionWidth,
        a,
        b);
    if (tmpConnection->orientation == "")
    {
        std::cout << "Connection not valid" << std::endl;
        delete tmpConnection;
    }
}

void World::selectConnectionA()
{
    std::vector<const char *> tmpLevels = getLevelNames();
    if (ImGui::BeginCombo("Level A: ", selectedConnectionA))
    {
        for (int n = 0; n < tmpLevels.size(); n++)
        {
            bool is_selected = (selectedConnectionA == tmpLevels[n]);
            if (ImGui::Selectable(tmpLevels[n], is_selected))
                selectedConnectionA = tmpLevels[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void World::selectConnectionB()
{
    std::vector<const char *> tmpLevels = getLevelNames();
    if (ImGui::BeginCombo("Level B: ", selectedConnectionB))
    {
        for (int n = 0; n < tmpLevels.size(); n++)
        {
            bool is_selected = (selectedConnectionB == tmpLevels[n]);
            if (ImGui::Selectable(tmpLevels[n], is_selected))
                selectedConnectionB = tmpLevels[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void World::updateConnection()
{
    tmpConnection->update(worldPosition);
    if (scene->window->isButtonPressed(GLFW_MOUSE_BUTTON_1))
    {
        connections.push_back(tmpConnection);
        tmpConnection = nullptr;
    }
}
