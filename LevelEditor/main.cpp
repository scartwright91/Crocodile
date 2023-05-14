
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
        if (inStartScreen && !startScreen->isActive())
        {
            inStartScreen = false;
            editor = new Editor(startScreen->getProject(), scene, &resourceManager);
            return;
        }
    }

    void renderImGui()
    {
        if (inStartScreen)
            startScreen->renderImGui();
        else
            editor->renderImGui();
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
