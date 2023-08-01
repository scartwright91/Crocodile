
#include "Crocodile.h"

using namespace Crocodile;

class CollisionExample : public Crocodile::Application
{

public:
    s2d::Text *fps = nullptr;
    s2d::Object *player = nullptr;
    s2d::Object *rect = nullptr;
    std::vector<s2d::shapes::Line *> collisionLines = {};
    std::vector<s2d::shapes::Line *> intersectionLines = {};

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
            player->rotate(-0.05f);
        else if (scene->window->isKeyPressed(GLFW_KEY_RIGHT))
            player->rotate(0.05f);

        player->move(dx, dy);

        // check collision
        if (player->getBoundingBox().intersectsBounds(rect->getBoundingBox()))
            player->color = glm::vec3(1.f, 0.f, 0.f);
        else
            player->color = glm::vec3(0.f);

        // add collision vectors
        s2d::col::BoundingBox bbox = rect->getBoundingBox();
        s2d::col::CollisionVectors cv = player->getBoundingBox().getCollisionVectors(&bbox);
        std::vector<s2d::col::Line> lines = cv.cornerProjections;
        for (unsigned int i = 0; i < lines.size(); i++)
        {
            collisionLines[i]->p1 = lines[i].origin * glm::vec2(1.5f);
            collisionLines[i]->p2 = lines[i].direction * glm::vec2(1.5f);
        }
        std::vector<s2d::col::LineIntersection> linesIntersections = cv.lineIntersections;
        for (unsigned int i = 0; i < linesIntersections.size(); i++)
        {
            intersectionLines[i]->p1 = linesIntersections[i].min * glm::vec2(1.5f);
            intersectionLines[i]->p2 = linesIntersections[i].max * glm::vec2(1.5f);
            if (linesIntersections[i].collision)
                intersectionLines[i]->color = glm::vec3(1.f, 0.f, 0.f);
            else
                intersectionLines[i]->color = glm::vec3(1.f, 0.6f, 0.f);
        }
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
        player->setPosition(glm::vec2(410.f));
        player->alpha = 0.3f;
        player->rotate(0.3f);
        player->size = glm::vec2(100.f, 60.f);
        player->color = glm::vec3(0.f);
        // player->showBoundingBox = true;

        rect = new s2d::Object();
        rect->alpha = 0.3f;
        // rect->rotate(0.7f);
        rect->setPosition(glm::vec2(500.f));
        rect->size = glm::vec2(100.f, 50.f);
        rect->color = glm::vec3(0.f);
        rect->showBoundingBox = true;

        window.setBackgroundColor(glm::vec3(0.7f));
        scene->camera->setTarget(player, false);
        scene->camera->setZoom(0.5f);
        scene->addChild(player, "entities");
        scene->addChild(rect, "entities");

        // add collision vectors
        s2d::col::BoundingBox bbox = rect->getBoundingBox();
        s2d::col::CollisionVectors cv = player->getBoundingBox().getCollisionVectors(&bbox);
        std::vector<s2d::col::Line> lines = cv.cornerProjections;
        for (s2d::col::Line line : lines)
        {
            s2d::shapes::Line *l = new s2d::shapes::Line(
                line.origin * glm::vec2(1.5f),
                line.direction * glm::vec2(1.5f));
            scene->addChild(l, "entities");
            collisionLines.push_back(l);
        }
        // line intersections
        std::vector<s2d::col::LineIntersection> intersections = cv.lineIntersections;
        for (s2d::col::LineIntersection intersection : intersections)
        {
            s2d::shapes::Line *l = new s2d::shapes::Line(
                intersection.min * glm::vec2(1.5f),
                intersection.max * glm::vec2(1.5f));
            l->color = glm::vec3(1.f, 0.f, 0.f);
            scene->addChild(l, "entities");
            intersectionLines.push_back(l);
        }
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new CollisionExample();
}
