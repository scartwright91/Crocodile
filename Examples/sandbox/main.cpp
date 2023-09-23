
#include "Crocodile.h"

using namespace Crocodile;

class Sandbox : public Crocodile::Application
{

public:
    Sandbox() : Crocodile::Application("Sandbox", true, 1280, 720, false)
    {
    }

    void update(float dt)
    {
    }

    void init()
    {
        // create hud layer
        s2d::Layer *hud = new s2d::Layer("hud");
        hud->applyZoom = false;
        hud->cameraScroll = false;
        scene->layerStack->addLayer(hud);
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
