#include "StartScreen.h"

StartScreen::StartScreen()
{
}

StartScreen::~StartScreen()
{
}

void StartScreen::renderImGui()
{
    ImGui::Begin("Project Selection");
    if (ImGui::Button("New Project", ImVec2(100, 50)))
        renderNewProjectOptions = true;
    ImGui::SameLine();
    if (ImGui::Button("Load Project", ImVec2(100, 50)))
        std::cout << "Open file dialog" << std::endl;
    ImGui::NewLine();
    if (renderNewProjectOptions)
    {
        ImGui::ShowDemoWindow();
        // ImGui::InputTextWithHint("project_name", projectName.c_str(), "Project name", 2 * 256);
    }
    ImGui::End();
}
