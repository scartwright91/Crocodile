
#include "Crocodile.h"

#include "LDtkLoader/Project.hpp"

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:


    Sandbox() : Crocodile::Application("Sandbox", false, 1280, 720, false)
    {
        init();
    }

    ~Sandbox()
    {
    }

    void update(float dt)
    {

    }

    void init()
    {

    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
