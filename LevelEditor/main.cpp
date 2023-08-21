
#include "Crocodile.h"

#include "src/StartScreen.h"
#include "src/Editor.h"

using namespace Crocodile;

class LevelEditor : public Crocodile::Application
{

public:
    bool inStartScreen = true;

    StartScreen *startScreen = nullptr;
    Editor *editor = nullptr;

    LevelEditor() : Crocodile::Application("Level Editor", true, 1200, 800, true)
    {
        init();
    }

    ~LevelEditor()
    {
        delete startScreen;
    }

    void update(float dt)
    {
        if (inStartScreen)
        {
            if (startScreen->isActive())
                return;
            else
            {
                inStartScreen = false;
                editor = new Editor(startScreen->getProject(&resourceManager), scene, &resourceManager);
            }
        }
        else
        {
            editor->update(dt, mouseOnImGuiWindow);
            fontImGuiScale = editor->fontImGuiScale;
        }
    }

    void renderImGui()
    {
        if (inStartScreen)
            startScreen->renderImGui();
        else
        {
            renderDockspace();
            editor->renderImGui();
        }
        ImGui::Render();
    }

    void renderDockspace()
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        static bool p_open = true;
        ImGui::Begin("DockSpace Demo", &p_open, window_flags);
        // Submit the DockSpace
        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        ImGui::End();
    }

    void init()
    {
        scene->window->setBackgroundColor(glm::vec3((float)16 / 255, (float)42 / 255, (float)67 / 255));
        scene->enableScaling = false;
        startScreen = new StartScreen();
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new LevelEditor();
}
