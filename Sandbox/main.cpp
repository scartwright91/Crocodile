
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "Crocodile.h"

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:

    s2d::Text* fps = new s2d::Text();

    Sandbox() : Crocodile::Application("Sandbox", false, 1280, 720, false)
    {

    }

    ~Sandbox()
    {
    }

    void update(float dt)
    {
        fps->text = std::to_string(clock.getFPS());
        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
            running = false;

    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
