#include "Editor.h"

Editor::Editor(
    Project *project,
    s2d::Scene *scene,
    ResourceManager *rm) : project(project), scene(scene), rm(rm)
{
    init();
    scene->layerStack->addLayer(new s2d::Layer("canvas"));
    scene->layerStack->addLayer(new s2d::Layer("canvas_edges"));
    scene->layerStack->addLayer(new s2d::Layer("hud"));
    if (project->loadProjectFromFile)
        levels.push_back(new Level(project->data, scene, rm));
    else
        levels.push_back(new Level("level0", scene, rm, glm::vec2(600.f)));
    activeLevel = levels[0];
}

void Editor::renderImGui()
{
    showImGuiMainMenu();
    if (activeLevel != NULL)
        activeLevel->renderImGui();
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Scene");
    ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
    if ((currentViewportSize.x != viewportSize.x) || (currentViewportSize.y != viewportSize.y))
    {
        viewportSize = glm::vec2(currentViewportSize.x, currentViewportSize.y);
        scene->postProcessing->resize((int)viewportSize.x, (int)viewportSize.y);
    }
    unsigned int textureID = scene->getTextureBuffer();
    ImGui::Image((void *)textureID, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
    if (ImGui::IsItemHovered())
    {
        ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
        ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
        sceneMousePosition = glm::vec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y + screenPositionAbsolute.y);
        mouseOnSceneWindow = true;
    }
    else
        mouseOnSceneWindow = false;
    ImGui::End();
    // ImGui::PopStyleVar();
}

void Editor::update(bool mouseOnImGuiWindow)
{
    this->mouseOnImGuiWindow = mouseOnImGuiWindow;
    zoom();
    move();
    if (activeLevel != NULL)
        activeLevel->update(sceneMousePosition, viewportSize);
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
        activeLevel->scaleEdges(1 + z);
    }
}

void Editor::move()
{
    if (!mouseOnSceneWindow)
        return;
    float zoomSpeed = speed * (scene->camera->zoom * 0.5);
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
    project->save(ld);
}

void Editor::load()
{
    project->load();
}

void Editor::showImGuiMainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Project"))
        {
            if (ImGui::MenuItem("Open", "CTRL O"))
            {
                load();
            }
            if (ImGui::MenuItem("Save", "CTRL S"))
            {
                save();
            }
            if (ImGui::MenuItem("Close"))
            {
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
            if (ImGui::MenuItem("Font+", "CTRL+"))
            {
                fontImGuiScale += 0.2f;
            }
            if (ImGui::MenuItem("Font-", "CTRL-"))
            {
                if (fontImGuiScale - 0.2 > 0.f)
                    fontImGuiScale -= 0.2f;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
