#include "Editor.h"

Editor::Editor(
    Project *project,
    s2d::Scene *scene,
    ResourceManager *rm) : project(project), scene(scene), rm(rm)
{
    init();
    scene->layerStack->addLayer(new s2d::Layer("canvas"));
    scene->layerStack->addLayer(new s2d::Layer("canvas_edges"));
    levels.push_back(new Level("level0", scene));
}

void Editor::renderImGui()
{
    // ImGui::ShowDemoWindow();
}

void Editor::update()
{
    zoom();
    move();
}

void Editor::zoom()
{
    float scroll = scene->window->scroll.y;
    float now = glfwGetTime();
    if (scroll != currentZoom)
    {
        // mouse position
        glm::vec2 mouse = scene->window->getMouseScreenPosition();
        glm::vec2 camPos = camera->getPosition();
        // camera->move(mouse.x - camPos.x, mouse.y - camPos.y);
        // camera->setPosition(mouse);
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

    if (scene->window->isKeyPressed(GLFW_KEY_W))
        dy = -speed;
    if (scene->window->isKeyPressed(GLFW_KEY_S))
        dy = speed;
    if (scene->window->isKeyPressed(GLFW_KEY_A))
        dx = -speed;
    if (scene->window->isKeyPressed(GLFW_KEY_D))
        dx = speed;
    camera->move(dx, dy);
}

void Editor::init()
{
    camera = new s2d::Object();
    camera->move(scene->windowWidth / 2, scene->windowHeight / 2);
    scene->camera->setZoom(4.0f);
    scene->camera->setTarget(camera, false);
    currentZoom = scene->window->scroll.y;
}
