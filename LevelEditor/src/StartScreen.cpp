#include "StartScreen.h"

StartScreen::StartScreen()
{
}

StartScreen::~StartScreen()
{
}

Project *StartScreen::getProject()
{
    return new Project(projectName, getProjectPath());
}

bool StartScreen::isActive()
{
    return active;
}

void StartScreen::renderImGui()
{
    ImGui::Begin("Project Selection");
    if (ImGui::Button("New Project", ImVec2(100, 50)))
    {
        renderNewProjectOptions = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Project", ImVec2(100, 50)))
        std::cout << "Open file dialog" << std::endl;
    ImGui::NewLine();
    if (renderNewProjectOptions)
    {
        ImGui::InputText("Project name", projectName, 32);
        ImGui::SameLine();
        if (ImGui::Button("Choose folder", ImVec2(100, 50)))
            ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", "Choose a Directory", nullptr, ".");
        ImGui::NewLine();
        ImGui::Text("Project path:");
        ImGui::SameLine();
        ImGui::Text(getProjectPath().c_str());
        ImGui::NewLine();
        if (ImGui::Button("Create project", ImVec2(100, 50)) && projectName != "")
            active = false;
    }
    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseDirDlgKey"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string path = ImGuiFileDialog::Instance()->GetCurrentPath();
            projectFolder = path + "\\";
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }
    ImGui::End();
}

std::string StartScreen::getProjectPath()
{
    std::string path = "";
    path += std::string(projectFolder);
    path += projectName;
    return path;
}
