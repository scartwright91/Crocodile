#include "Editor.h"

Editor::Editor(
    Project *project,
    s2d::Scene *scene,
    ResourceManager *rm) : project(project), scene(scene), rm(rm)
{
    init();
    scene->layerStack->addLayer(new s2d::Layer("canvas"));
    scene->layerStack->addLayer(new s2d::Layer("canvas_edges"));
    levels.push_back(new Level("level0", scene, rm, glm::vec2(600.f)));
}

void Editor::renderImGui()
{
    showImGuiMainMenu();
    for (Level *level : levels)
        level->renderImGui();
}

void Editor::update()
{
    zoom();
    move();
    for (Level *level : levels)
        level->update();
}

void Editor::zoom()
{
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
    }
}

void Editor::move()
{
    float speed = 50.f;
    float dx = 0.f;
    float dy = 0.f;

    if (scene->window->isKeyPressed(GLFW_KEY_UP))
        dy = -speed;
    if (scene->window->isKeyPressed(GLFW_KEY_DOWN))
        dy = speed;
    if (scene->window->isKeyPressed(GLFW_KEY_LEFT))
        dx = -speed;
    if (scene->window->isKeyPressed(GLFW_KEY_RIGHT))
        dx = speed;
    camera->move(dx, dy);
}

void Editor::init()
{
    camera = new s2d::Object();
    camera->move(scene->windowWidth / 2, scene->windowHeight / 2);
    // scene->camera->setZoom(4.0f);
    scene->camera->setTarget(camera, false);
    currentZoom = scene->window->scroll.y;
}

void Editor::showImGuiMainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Project"))
        {
            if (ImGui::MenuItem("Open", "CTRL+O"))
            {
            }
            if (ImGui::MenuItem("Save", "CTRL+S"))
            {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z"))
            {
            }
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
            {
            } // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X"))
            {
            }
            if (ImGui::MenuItem("Copy", "CTRL+C"))
            {
            }
            if (ImGui::MenuItem("Paste", "CTRL+V"))
            {
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
