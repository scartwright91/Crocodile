
#include "Crocodile.h"

#include "LDtkLoader/Project.hpp"

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:

    s2d::Object* obj = new s2d::Object();
    s2d::Object* obj2 = new s2d::Object();

    Sandbox() : Crocodile::Application("Sandbox", false, 1280, 720, false)
    {
        init();
    }

    ~Sandbox()
    {
        delete obj;
    }

    void update(float dt)
    {

    }

    void init()
    {
        s2d::Layer* layer = new s2d::Layer("objects");
        scene2d->layerStack->addLayer(layer);

        obj->setPosition(glm::vec2(400.f));
        obj->size = glm::vec2(50.f);
        obj->color = glm::vec3(1.f, 0.f, 0.f);
        
        obj2->setPosition(glm::vec2(50.f));
        obj2->size = glm::vec2(10.f);
        obj2->color = glm::vec3(1.f, 0.f, 1.f);
        
        obj->addChildObject(obj2);

        scene2d->addObject(obj, "objects");
        scene2d->camera->setTarget(obj, false);
    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
