
#include "Crocodile.h"

#include "box2d/box2d.h"

using namespace Crocodile;

class Box2D : public Crocodile::Application
{

public:
    s2d::Text *fps = nullptr;

    Box2D() : Crocodile::Application("Box2D example", true, 1280, 720, false)
    {
        init();
        b2Vec2 gravity(0.f, -10.f);
        b2World world(gravity);

        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -10.0f);

        b2Body *groundBody = world.CreateBody(&groundBodyDef);

        b2PolygonShape groundBox;
        groundBox.SetAsBox(50.0f, 10.0f);

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(0.0f, 4.0f);
        b2Body *body = world.CreateBody(&bodyDef);

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(1.0f, 1.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);

        int32 velocityIterations = 6;
        int32 positionIterations = 2;

        float timeStep = 1.0f / 60.0f;
        for (int32 i = 0; i < 60; ++i)
        {
            world.Step(timeStep, velocityIterations, positionIterations);
            b2Vec2 position = body->GetPosition();
            float angle = body->GetAngle();
            printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
        }
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
