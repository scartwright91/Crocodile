
#include "Crocodile.h"

using namespace Crocodile;

class Sandbox : public Crocodile::Application
{

public:
    s2d::shapes::Circle *circle = nullptr;

    Sandbox() : Crocodile::Application("Sandbox", true, 1280, 720, false)
    {
        s2d::Layer *hud = new s2d::Layer("hud");
        hud->applyZoom = false;
        hud->cameraScroll = false;
        scene->layerStack->addLayer(hud);

        circle = new s2d::shapes::Circle(5.f);
        circle->size = glm::vec2(200.f);
        circle->color = glm::vec3(1.f);
        circle->setPosition(glm::vec2(200.f));
        scene->addObject(circle, "hud");
    }

    void update(float dt)
    {
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
