
#include "Crocodile.h"

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:

    s2d::Text* fps = new s2d::Text();

    Sandbox() : Crocodile::Application("Sandbox", false, 1280, 720, false)
    {
        init();
    }

    ~Sandbox()
    {
    }

    void update(float dt)
    {
        fps->text = std::to_string((int)clock.getFPS());
    }

    void fixedUpdate(float dt)
    {
    }

    void init()
    {
        window.setBackgroundColor(glm::vec3(0.f, 0.5f, 0.7f));

        // create hud and fps text
        s2d::Layer *hud = new s2d::Layer("hud");
        hud->applyZoom = false;
        hud->cameraScroll = false;
        scene->layerStack->addLayer(hud);
        scene->addObject(fps, "hud");

        // create layer and add to scene
        s2d::Layer *layer = new s2d::Layer("objects");
        scene->layerStack->addLayer(layer);

        // create square and to objects layer
        s2d::Object *square = new s2d::Object();
        square->size = glm::vec2(100.f);
        square->color = glm::vec3(1.f, 0.f, 1.f);
        scene->addObject(square, "objects");

        // create circle
        s2d::shapes::Circle* circle = new s2d::shapes::Circle(50.f);
        circle->setPosition(glm::vec2(100.f));
        circle->color = glm::vec3(1.f);
        scene->addObject(circle, "objects");

        // set the camera's focus to the square
        scene->camera->setTarget(square, false);

    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
