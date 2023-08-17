#include "Editor.h"

Editor::Editor(
    Project *project,
    s2d::Scene *scene,
    ResourceManager *rm) : project(project), scene(scene), rm(rm)
{
    init();
    scene->layerStack->addLayer(new s2d::Layer("canvas"));
    scene->layerStack->addLayer(new s2d::Layer("grid"));
    scene->layerStack->addLayer(new s2d::Layer("canvas_edges"));
    scene->layerStack->addLayer(new s2d::Layer("hud"));
    if (project->loadProjectFromFile)
        levels.push_back(new Level(project->data, scene, rm));
    else
    {
        levels.push_back(new Level("level0", scene, rm, glm::vec2(0.f), glm::vec2(600.f)));
        levels.push_back(new Level("level1", scene, rm, glm::vec2(1000.f), glm::vec2(2000.f)));
    }
    activeLevel = levels[0];
}

void Editor::renderImGui()
{
    showImGuiMainMenu();
    if (showParticleEditor)
        showParticleEditor = showImGuiParticleEditor(showParticleEditor);
    if (worldView)
    {
    }
    else
    {
        if (activeLevel != NULL)
            activeLevel->renderImGui();
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Scene");
    ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
    if ((currentViewportSize.x != viewportSize.x) || (currentViewportSize.y != viewportSize.y))
    {
        viewportSize = glm::vec2(currentViewportSize.x, currentViewportSize.y);
        scene->postProcessing->resize((int)viewportSize.x, (int)viewportSize.y);
    }
    unsigned int textureID = scene->getTextureBuffer();
    ImGui::Image((void *)(intptr_t)textureID, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
    if (ImGui::IsItemHovered())
    {
        // calculate mouse position relative to scene
        ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
        ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        ImVec2 viewportOffset = ImGui::GetWindowPos();
        glm::vec2 bounds = glm::vec2(viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y);
        ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
        sceneMousePosition = glm::vec2(mousePositionAbsolute.x, mousePositionAbsolute.y);
        sceneMousePosition.x -= bounds.x;
        sceneMousePosition.y -= bounds.y;
        mouseOnSceneWindow = true;
    }
    else
        mouseOnSceneWindow = false;
    ImGui::End();
    ImGui::PopStyleVar();
}

void Editor::update(float dt, bool mouseOnImGuiWindow)
{
    this->mouseOnImGuiWindow = mouseOnImGuiWindow;
    zoom();
    move(dt);
    if (worldView)
    {
        for (Level *level : levels)
            level->update(dt, sceneMousePosition);
    }
    else
    {
        if (activeLevel != NULL)
            activeLevel->update(dt, sceneMousePosition);
    }
}

void Editor::zoom()
{
    if (!mouseOnSceneWindow)
        return;
    float scroll = scene->window->scroll.y;
    float now = glfwGetTime();
    if (scroll != currentZoom)
    {
        float z = abs(scroll) / 10;
        if (scroll > currentZoom)
            scene->camera->changeZoom(-z);
        else
            scene->camera->changeZoom(z);
        currentZoom = scroll;
        zoomTimer = now;
        activeLevel->canvas->scaleEdges();
    }
}

void Editor::move(float dt)
{
    if (!mouseOnSceneWindow)
        return;
    float zoomSpeed = speed * (scene->camera->zoom * 0.5) * dt;
    float dx = 0.f;
    float dy = 0.f;

    if (scene->window->isKeyPressed(GLFW_KEY_UP))
        dy = -zoomSpeed;
    if (scene->window->isKeyPressed(GLFW_KEY_DOWN))
        dy = zoomSpeed;
    if (scene->window->isKeyPressed(GLFW_KEY_LEFT))
        dx = -zoomSpeed;
    if (scene->window->isKeyPressed(GLFW_KEY_RIGHT))
        dx = zoomSpeed;
    camera->move(dx, dy);
}

void Editor::init()
{
    camera = new s2d::Object();
    camera->move(scene->windowWidth / 2, scene->windowHeight / 2);
    scene->camera->setTarget(camera, false);
    currentZoom = scene->window->scroll.y;
}

void Editor::save()
{
    LevelData ld = activeLevel->serialise();
    std::cout << "Project saved" << std::endl;
    project->save(ld);
}

void Editor::load()
{
    if (activeLevel != NULL)
        activeLevel->load();
    std::cout << "Load not implemented" << std::endl;
}

void Editor::close()
{
    if (activeLevel != NULL)
        activeLevel->clear();
    std::cout << "Close not implemented" << std::endl;
}

void Editor::showImGuiMainMenu()
{
    float now = glfwGetTime();
    // user commands
    bool uSave = scene->window->isKeyPressed(GLFW_KEY_LEFT_CONTROL) && scene->window->isKeyPressed(GLFW_KEY_S);
    bool uSwitch = scene->window->isKeyPressed(GLFW_KEY_TAB);
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Project"))
        {
            if ((ImGui::MenuItem("Save", "CTRL S") || uSave) && (now - commandTimer > 0.5f))
            {
                save();
                commandTimer = now;
            }
            if (ImGui::MenuItem("Load", "CTRL O"))
            {
                load();
            }
            if (ImGui::MenuItem("Clear"))
            {
                close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL Z"))
            {
            }
            if (ImGui::MenuItem("Redo", "CTRL Y"))
            {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if ((ImGui::MenuItem("Switch view", "Tab") || uSwitch) && (now - commandTimer > 0.5f))
            {
                switchView();
                commandTimer = now;
            }
            if (ImGui::MenuItem("Font +", "CTRL+"))
            {
                fontImGuiScale += 0.2f;
            }
            if (ImGui::MenuItem("Font -", "CTRL-"))
            {
                if (fontImGuiScale - 0.2 > 0.f)
                    fontImGuiScale -= 0.2f;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Particle editor"))
            {
                // showParticleEditor = true;
                std::cout << "Not implemented" << std::endl;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

bool Editor::showImGuiParticleEditor(bool show)
{
    ImGui::Begin("Particle editor", &show);
    ImGui::Text("Not implemented");
    ImGui::End();
    return show;
}

void Editor::switchView()
{
    if (worldView)
    {
        worldView = false;
        activeLevel->load();
    }
    else
    {
        worldView = true;
        activeLevel->clear();
    }
}
