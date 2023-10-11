
#include "Crocodile.h"

using namespace Crocodile;

class Sandbox : public Crocodile::Application
{

public:
    Sandbox() : Crocodile::Application("Sandbox", true, 1280, 720, true)
    {
        init();
    }

    ~Sandbox()
    {
    }

    void update(float dt)
    {
    }

    void fixedUpdate(float dt)
    {
    }

    void renderImGui()
    {
        ImGui::Render();
    }

    void init()
    {
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
