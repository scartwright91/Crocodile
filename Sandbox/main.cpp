
#include "Crocodile.h"
#include "Crocodile/s2d/Particles.h"

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

        float now = (float)glfwGetTime();
        if (window.isButtonPressed(GLFW_MOUSE_BUTTON_1) && (now - timer > 0.5f))
        {
            timer = now;
            s2d::ParticleSettings settings;
            settings.amount = 500;
            settings.life = .5f;
            settings.scale = 2.0f;
            settings.direction = 1.4f;
            // settings.dispersion = 1.5f;
            settings.createOnce = false;
            settings.type = s2d::RADIAL;
            // settings.applyGravity = true;
            settings.colour = glm::vec3(glm::cos(elapsed), glm::sin(elapsed), glm::tan(elapsed));
            scene2d->addParticleEffect(window.getMouseScreenPosition(), settings, "hud");
        }

        scene3d->camera->position.z += dt;
    }

    void init()
    {
        window.setBackgroundColor(glm::vec3(0.02f, 0.13f, 0.22f));

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
        
    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
