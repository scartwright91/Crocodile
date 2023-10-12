
#include "Crocodile.h"

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

    void fixedUpdate(float dt)
    {
    }

    void init()
    {
        window.setBackgroundColor(glm::vec3(0.f, 0.5f, 0.7f));

        // create layer and add to scene
        s2d::Layer *layer = new s2d::Layer("objects");
        scene->layerStack->addLayer(layer);

        // create square and to objects layer
        s2d::Object *square = new s2d::Object();
        square->size = glm::vec2(100.f);
        square->color = glm::vec3(1.f, 0.f, 1.f);
        scene->addObject(square, "objects");

        // set the camera's focus to the square
        scene->camera->setTarget(square, false);
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
