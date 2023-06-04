#include "StartScreen.h"

StartScreen::StartScreen()
{
}

StartScreen::~StartScreen()
{
}

Project *StartScreen::getProject(ResourceManager *rm)
{
    return new Project(projectName, getProjectPath(), loadProjectFromFile, rm);
}

bool StartScreen::isActive()
{
    return active;
}

void StartScreen::renderImGui()
{
    ImGui::Begin("Project Selection");
    if (ImGui::Button("Recent", ImVec2(100, 50)))
    {
        loadProjectFromFile = true;
        renderOptions = "recent_projects";
    }
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 128, 0, 150));
    if (ImGui::Button("New Project", ImVec2(100, 50)))
    {
        loadProjectFromFile = false;
        renderOptions = "new_project";
    }
    ImGui::PopStyleColor();
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 172, 28, 150));
    if (ImGui::Button("Load Project", ImVec2(100, 50)))
    {
        loadProjectFromFile = true;
        renderOptions = "load_project";
    }
    ImGui::PopStyleColor();
    ImGui::NewLine();
    if (renderOptions == "recent_projects")
        recentProjects();
    else if (renderOptions == "new_project")
        newProject();
    else if (renderOptions == "load_project")
        loadProject();
    ImGui::End();
}

std::string StartScreen::getProjectPath()
{
    std::string path = "";
    path += std::string(projectFolder);
    if (loadProjectFromFile)
        path += projectNameFromFile;
    else
        path += projectName;
    return path;
}

void StartScreen::recentProjects()
{
    ImGui::Text("Recent projects");
}

void StartScreen::newProject()
{
    ImGui::Text("New project");
    ImGui::InputText("Project name", projectName, 32);
    if (ImGui::Button("Choose folder", ImVec2(100, 50)))
        ImGuiFileDialog::Instance()->OpenDialog("choose_folder", "Choose a Directory", nullptr, ".");
    ImGui::NewLine();
    ImGui::Text("Project path:");
    ImGui::SameLine();
    ImGui::Text(getProjectPath().c_str());
    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::NewLine();
    if (ImGui::Button("Create project", ImVec2(100, 50)) && projectName != "")
        active = false;

    // display
    if (ImGuiFileDialog::Instance()->Display("choose_folder"))
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
}

void StartScreen::loadProject()
{
    ImGui::Text("Load project");
    if (ImGui::Button("Choose project", ImVec2(100, 50)))
        ImGuiFileDialog::Instance()->OpenDialog("choose_project", "Choose a Directory", ".cld", ".");
    ImGui::Text("Project path:");
    ImGui::SameLine();
    ImGui::Text(getProjectPath().c_str());
    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::NewLine();
    if (ImGui::Button("Load project", ImVec2(100, 50)) && projectName != "")
        active = false;
    // display
    if (ImGuiFileDialog::Instance()->Display("choose_project"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string path = ImGuiFileDialog::Instance()->GetCurrentPath();
            projectNameFromFile = ImGuiFileDialog::Instance()->GetCurrentFileName();
            projectFolder = path + "\\";
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }
}
