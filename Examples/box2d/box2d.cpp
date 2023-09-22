
#include "Crocodile.h"

using namespace Crocodile;

class Box2D : public Crocodile::Application
{

public:
    s2d::Text *fps = nullptr;

    Box2D() : Crocodile::Application("Box2D example", true, 1280, 720, false)
    {
        init();
    }

    void update(float dt)
    {
        fps->text = std::to_string(clock.getFPS());
    }

    void init()
    {
        // create hud layer
        s2d::Layer *hud = new s2d::Layer("hud");
        hud->applyZoom = false;
        hud->cameraScroll = false;
        scene->layerStack->addLayer(hud);

        // create entities layer
        s2d::Layer *entLayer = new s2d::Layer("entities");
        scene->layerStack->addLayer(entLayer);

        // fps text
        fps = new s2d::Text();
        fps->text = "";
        fps->color = glm::vec3(1.f);
        scene->addObject(fps, "hud");
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Box2D();
}
