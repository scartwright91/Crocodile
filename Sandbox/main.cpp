
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
        s3d::Object* obj = new s3d::Object(glm::vec3(0.f), glm::vec3(10.f));
        scene3d->addObject(obj);
    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
