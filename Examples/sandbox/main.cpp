
#include "Crocodile.h"

using namespace Crocodile;

class Sandbox : public Crocodile::Application
{

public:
    s2d::shapes::Circle *circle = nullptr;
    s2d::Object *test = nullptr;

    Sandbox() : Crocodile::Application("Sandbox", true, 1280, 720, false)
    {
        window.setBackgroundColor(glm::vec3(1.f));

        s2d::Layer *hud = new s2d::Layer("hud");
        hud->applyZoom = false;
        hud->cameraScroll = false;
        scene->layerStack->addLayer(hud);

        circle = new s2d::shapes::Circle(50.f);
        circle->color = glm::vec3(1.f, 0.f, 0.f);
        circle->setPosition(glm::vec2(200.f));
        scene->addObject(circle, "hud");

        test = new s2d::Object();
        test->size = glm::vec2(100.f);
        test->color = glm::vec3(1.f, 1.f, 0.f);
        scene->addObject(test, "hud");
    }

    void update(float dt)
    {
        circle->setPosition(window.getMouseScreenPosition() + circle->size * 0.5f);
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
