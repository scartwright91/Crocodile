
#include "Crocodile.h"

#include "LDtkLoader/Project.hpp"

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:

    s2d::Object* square = new s2d::Object();
    s2d::Text* fps = new s2d::Text();

    Sandbox() : Crocodile::Application("Sandbox", false, 1280, 720, false)
    {
        init();
        parseLevel("Level_0");
    }

    ~Sandbox()
    {
    }

    void update(float dt)
    {
        fps->text = std::to_string((int)clock.getFPS());

        glm::vec2 mousePos = window.getMouseScreenPosition();
        glm::vec2 center = glm::vec2(scene->windowWidth / 2, scene->windowHeight / 2);
        float theta = glm::atan(mousePos.y - center.y, mousePos.x - center.x);
        square->velocity = glm::vec2(glm::cos(theta) * 5.f, glm::sin(theta) * 5.f);
    }

    void init()
    {
        resourceManager.loadTexture("res/tileset.png", "tileset", false);
        window.setBackgroundColor(glm::vec3(0.f, 0.5f, 0.7f));

        // create hud and fps text
        s2d::Layer *hud = new s2d::Layer("hud");
        hud->applyCamera = false;
        scene->layerStack->addLayer(hud);
        scene->addObject(fps, "hud");

        // create layer and add to scene
        s2d::Layer *layer = new s2d::Layer("objects");
        scene->layerStack->addLayer(layer);

        // create square and to objects layer
        square->size = glm::vec2(100.f);
        square->color = glm::vec3(1.f, 0.f, 1.f);
        scene->addObject(square, "objects");
        square->collisionLayers.push_back(0);

        // create circle
        s2d::shapes::Circle* circle = new s2d::shapes::Circle(50.f);
        circle->setPosition(glm::vec2(100.f));
        circle->color = glm::vec3(1.f);
        scene->addObject(circle, "objects");

        // set the camera's focus to the square
        scene->camera->setTarget(square, false);

    }

    void parseLevel(std::string levelName)
    {
        ldtk::Project project;
        project.loadFromFile("res/world.ldtk");
        ResourceManager::TextureData tileset = resourceManager.getTexture("tilesset");

        // get the world
        const auto& world = project.getWorld();

        // get the level and the layer we want to render
        const auto& level = world.getLevel(levelName);
        const auto& layer = level.getLayer("Collisions");

        const auto& tiles_vector = layer.allTiles();
        for (const auto& tile : tiles_vector)
        {
            auto pos = tile.getPosition();
            auto tex = tile.getTextureRect();
            s2d::Object *obj = new s2d::Object();
            obj->setPosition(glm::vec2(pos.x, pos.y));
            obj->setTileMapTexture(
                tileset,
                32,
                tex.x,
                tex.y);
            obj->size = glm::vec2(tex.width, tex.height);
            scene->addObject(obj, "objects");
            scene->addObjectToCollisionLayer(obj, 0);
        }

    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
