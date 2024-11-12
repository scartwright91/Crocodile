
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "Crocodile.h"
// #include "Crocodile/s3d/Model.h"

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:

    s2d::Text* fps = new s2d::Text();

    Sandbox() : Crocodile::Application("Sandbox", true, 1280, 720, false, false)
    {
        init();
    }

    ~Sandbox()
    {
        delete fps;
    }

    void update(float dt)
    {
        fps->text = std::to_string((int)m_clock.getFPS());
        if (m_window.isKeyPressed(GLFW_KEY_ESCAPE))
            m_running = false;
    }

    void init()
    {
        m_resourceManager.addDirWatcher("res/shaders", AssetType::SHADER);

        s2d::Layer* layer = new s2d::Layer("HUD");
        layer->m_applyCamera = false;
        m_scene2d->m_layerStack->addLayer(layer);

        fps->m_color = glm::vec3(1.0f);
        m_scene2d->addObject(fps, "HUD");

        // s3d::Model* model = new s3d::Model("assets/models/survival-guitar-backpack/source/Survival_BackPack_2/Survival_BackPack_2.fbx");
        // m_scene3d->addObject(model);
    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
