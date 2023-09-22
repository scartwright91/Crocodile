
#include "Crocodile.h"

#include "box2d/box2d.h"

using namespace Crocodile;

class Box2D : public Crocodile::Application
{

public:
    s2d::Text *fps = nullptr;

    s2d::Object *cam = nullptr;
    s2d::Object *ground = nullptr;
    s2d::Object *rect = nullptr;

    b2World *world = nullptr;
    b2Body *body = nullptr;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    float timeStep = 1.0f / 60.0f;

    float timer = 0.0f;

    Box2D() : Crocodile::Application("Box2D example", true, 1280, 720, false)
    {
        init();
        b2Vec2 gravity(0.f, -10.f);
        world = new b2World(gravity);

        // ground definition
        ground = new s2d::Object();
        ground->size = glm::vec2(500, 100);
        ground->setPosition(glm::vec2(200.f, 600.f));
        ground->color = glm::vec3(1.f);
        scene->addObject(ground, "entities");

        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -10.0f);
        b2Body *groundBody = world->CreateBody(&groundBodyDef);

        b2PolygonShape groundBox;
        groundBox.SetAsBox(50.0f, 10.0f);
        groundBody->CreateFixture(&groundBox, 0.0f);

        // rect definition
        rect = new s2d::Object();
        rect->size = glm::vec2(100.f);
        rect->setPosition(glm::vec2(400.f, 200.f));
        rect->color = glm::vec3(1.f, 0.f, 0.f);
        scene->addObject(rect, "entities");

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(0.0f, 4.0f);
        body = world->CreateBody(&bodyDef);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(1.0f, 1.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);
    }

    void update(float dt)
    {
        timer += dt;
        fps->text = std::to_string(clock.getFPS());

        world->Step(timeStep, velocityIterations, positionIterations);
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
        rect->setPosition(glm::vec2(position.x, position.y * 100));
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

        cam = new s2d::Object();
        cam->setPosition(glm::vec2(1280 / 2, 720 * 0.75));
        scene->camera->setTarget(cam, false);
        scene->camera->setZoom(3.f);
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Box2D();
}
