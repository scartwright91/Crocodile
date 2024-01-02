
#include "Crocodile.h"
#include "Crocodile/s2d/Particles.h"
#include "Crocodile/s3d/Surface.h"

#include "LDtkLoader/Project.hpp"

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:

    float timer = 0.0f;
    float elapsed = 0.0f;

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
        elapsed += dt;
        fps->text = std::to_string(clock.getFPS());
        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
            running = false;

        scene3d->camera->position.z += dt;
    }

    void init()
    {
        window.setBackgroundColor(glm::vec3(0.02f, 0.13f, 0.22f));
        scene2d->enablePostprocessing = false;

        s2d::Layer* layer = new s2d::Layer("hud");
        layer->applyCamera = false;
        scene2d->layerStack->addLayer(layer);

        fps->color = glm::vec3(1.f);
        scene2d->addObject(fps, "hud");

        // create cubes
        std::vector<glm::vec3> cubes = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        for (glm::vec3 cube : cubes)
        {
            s3d::Object* obj = new s3d::Object(cube);
            scene3d->addObject(obj);
        }
        
        // add surface
        s3d::Surface* surface = new s3d::Surface(resourceManager.getTexture("heightmap"), resourceManager.getShader("surface_shader"));
        scene3d->surfaces.push_back(surface);

    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
