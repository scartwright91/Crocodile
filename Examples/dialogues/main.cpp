
#include "Crocodile.h"

#include "Crocodile/s2d/UIContainer.h"

using namespace Crocodile;

class DialoguesExample : public Crocodile::Application
{

public:
    s2d::UIContainer *container = nullptr;

    DialoguesExample() : Crocodile::Application("Dialogues example", true, 1280, 720, false)
    {
        init();
    }

    void update(float dt)
    {
    }

    void init()
    {
        window.setBackgroundColor(glm::vec3(0.82f));

        // create hud layer
        s2d::Layer *hud = new s2d::Layer("text_layer");
        hud->applyZoom = false;
        hud->cameraScroll = false;
        scene->layerStack->addLayer(hud);

        container = new s2d::UIContainer(glm::vec2(0.f));
        // container->setPosition(glm::vec2(50.f));
        container->color = glm::vec3(1.f);
        scene->addChild(container, "text_layer");

        s2d::Text *txt1 = new s2d::Text("Some more text to display", false);
        txt1->showBoundingBox = true;
        // s2d::Text *txt2 = new s2d::Text("Some more text to display", false);
        // txt2->showBoundingBox = true;
        container->addElementToUI(txt1);
        // container->addElementToUI(txt2);
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new DialoguesExample();
}
