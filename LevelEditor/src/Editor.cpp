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

    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    static bool p_open = true;
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    ImGui::End();
    showImGuiMainMenu();
    if (activeLevel != NULL)
        activeLevel->renderImGui();
    ImGui::Begin("Scene");
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    unsigned int textureID = scene->getTextureBuffer();
    ImGui::Image((void *)textureID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void Editor::update(bool mouseOnImGuiWindow)
{
    this->mouseOnImGuiWindow = mouseOnImGuiWindow;
    zoom();
    move();
    if (activeLevel != NULL)
        activeLevel->update();
}

void Editor::zoom()
{
    // if (mouseOnImGuiWindow)
    //     return;
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
    // if (mouseOnImGuiWindow)
    //     return;
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
