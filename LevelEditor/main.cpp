
#include "Crocodile.h"

#include "src/StartScreen.h"

using namespace Crocodile;

class LevelEditor : public Crocodile::Application
{

public:
    StartScreen *startScreen = nullptr;
    std::string projectName = "";

    LevelEditor() : Crocodile::Application("Level Editor", true, 1200, 800)
    {
        init();
    }

    ~LevelEditor()
    {
        delete startScreen;
    }

    void update(float dt)
    {
    }

    void renderImGui()
    {
        startScreen->renderImGui();
        ImGui::Render();
    }

    void init()
    {
        scene->window->setBackgroundColor(glm::vec3((float)16 / 255, (float)42 / 255, (float)67 / 255));
        startScreen = new StartScreen();
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new LevelEditor();
}
