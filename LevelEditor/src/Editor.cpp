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
    world = project->loadProjectFromFile ? new World(project->data, scene, rm) : new World(scene, rm);
}

Editor::~Editor()
{
    for (Level *level : levels)
        delete level;
    levels.clear();
    delete world;
}

void Editor::renderImGui()
{
    showImGuiMainMenu();
    if (worldView)
        world->renderImGui();
    else
        world->activeLevel->renderImGui();
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
    float now = glfwGetTime();
    this->mouseOnImGuiWindow = mouseOnImGuiWindow;
    world->mouseOnImGuiWindow = mouseOnImGuiWindow;
    zoom();
    move(dt);
    if (worldView)
    {
        world->update(dt, sceneMousePosition);
        if (world->activeLevel != NULL)
            useLevelView();
    }
    else
    {
        world->activeLevel->update(dt, sceneMousePosition);
        // switch to world view
        if (scene->window->isKeyPressed(GLFW_KEY_ESCAPE) && (now - commandTimer > 0.5f))
        {
            useWorldView();
            commandTimer = now;
        }
        if (scene->window->isKeyPressed(GLFW_KEY_TAB) && (now - commandTimer > 0.5f))
        {
            if (world->activeLevel->cameraView)
            {
                world->activeLevel->cameraView = false;
                scene->camera->setTarget(camera, true);
                scene->camera->setZoom(4.f);
            }
            else
            {
                world->activeLevel->cameraView = true;
                scene->camera->setTarget(world->activeLevel->camera, true);
                scene->camera->setZoom(1.f);
            }
            commandTimer = now;
        }
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
        commandTimer = now;
        float z = abs(scroll) / 10;
        if (scroll > currentZoom)
            scene->camera->changeZoom(-z);
        else
            scene->camera->changeZoom(z);
        currentZoom = scroll;
        zoomTimer = now;
        for (Level *level : levels)
            level->canvas->scaleEdges();
    }
}

void Editor::move(float dt)
{
    if (!mouseOnSceneWindow)
        return;
    float zoomSpeed = speed * (scene->camera->zoom * 0.5) * dt;
    float dx = 0.f;
    float dy = 0.f;

    if (scene->window->isKeyPressed(GLFW_KEY_UP) || scene->window->isKeyPressed(GLFW_KEY_W))
        dy = -zoomSpeed;
    if (scene->window->isKeyPressed(GLFW_KEY_DOWN) || scene->window->isKeyPressed(GLFW_KEY_S))
        dy = zoomSpeed;
    if (scene->window->isKeyPressed(GLFW_KEY_LEFT) || scene->window->isKeyPressed(GLFW_KEY_A))
        dx = -zoomSpeed;
    if (scene->window->isKeyPressed(GLFW_KEY_RIGHT) || scene->window->isKeyPressed(GLFW_KEY_D))
        dx = zoomSpeed;

    if (worldView)
        camera->move(dx, dy);
    else
    {
        if (world->activeLevel->cameraView)
            world->activeLevel->camera->move(dx, dy);
        else
            camera->move(dx, dy);
    }
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
    WorldData wd = world->serialise();
    project->save(wd);
    std::cout << "Project saved" << std::endl;
}

void Editor::load()
{
    std::cout << "Load not implemented" << std::endl;
}

void Editor::close()
{
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
        ImGui::EndMainMenuBar();
    }
}

void Editor::useWorldView()
{
    worldView = true;
    world->activeLevel->clear();
    world->activeLevel = nullptr;
    world->selectedLevel = nullptr;
}

void Editor::useLevelView()
{
    worldView = false;
    world->activeLevel->load();
    world->selectedLevel = nullptr;
}
