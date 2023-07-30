
#include "Crocodile.h"

using namespace Crocodile;

class CollisionExample : public Crocodile::Application
{

public:
    s2d::Text *fps = nullptr;
    s2d::Object *player = nullptr;
    s2d::Object *rect = nullptr;

    CollisionExample() : Crocodile::Application("Collision example", false, 1280, 720, false)
    {
        init();
    }

    void update(float dt)
    {
        fps->text = std::to_string(clock.getFPS());

        // move
        float speed = 400.f;
        float dx = 0.f;
        float dy = 0.f;
        if (scene->window->isKeyPressed(GLFW_KEY_W))
            dy = -speed * dt;
        if (scene->window->isKeyPressed(GLFW_KEY_S))
            dy = speed * dt;
        if (scene->window->isKeyPressed(GLFW_KEY_A))
            dx = -speed * dt;
        if (scene->window->isKeyPressed(GLFW_KEY_D))
            dx = speed * dt;

        if (scene->window->isKeyPressed(GLFW_KEY_LEFT))
            player->rotate(-0.01f);
        else if (scene->window->isKeyPressed(GLFW_KEY_RIGHT))
            player->rotate(0.01f);

        player->move(dx, dy);

        // check collision
        if (player->getBoundingBox().intersectsBounds(rect->getBoundingBox()))
            player->color = glm::vec3(1.f, 0.f, 0.f);
        else
            player->color = glm::vec3(0.f);
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
        scene->addChild(fps, "hud");

        player = new s2d::Object();
        player->size = glm::vec2(100.f);
        player->color = glm::vec3(0.f);
        player->showBoundingBox = true;

        rect = new s2d::Object();
        rect->setPosition(glm::vec2(500.f));
        rect->size = glm::vec2(100.f);
        rect->color = glm::vec3(0.1f);
        rect->showBoundingBox = true;

        window.setBackgroundColor(glm::vec3(0.88f));
        scene->camera->setTarget(player, false);
        // scene->camera->setZoom(2.f);
        scene->addChild(player, "entities");
        scene->addChild(rect, "entities");
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new CollisionExample();
}
