
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "Crocodile.h"
#include "Crocodile/s3d/Model.h"

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

        processCommands(dt);
    }

    void processCommands(float dt)
    {
        float speed = .05f * dt;

        // moving camera
        if (m_window.isKeyPressed(GLFW_KEY_A))
            m_scene3d->m_camera->move(s3d::LEFT, speed);
        if (m_window.isKeyPressed(GLFW_KEY_D))
            m_scene3d->m_camera->move(s3d::RIGHT, speed);
        if (m_window.isKeyPressed(GLFW_KEY_W))
            m_scene3d->m_camera->move(s3d::FORWARD, speed);
        if (m_window.isKeyPressed(GLFW_KEY_S))
            m_scene3d->m_camera->move(s3d::BACKWARD, speed);

        // rotating camera
        float rot = 100.f * dt;
        if (m_window.isKeyPressed(GLFW_KEY_LEFT))
            m_scene3d->m_camera->rotateYaw(rot);
        if (m_window.isKeyPressed(GLFW_KEY_RIGHT))
            m_scene3d->m_camera->rotateYaw(-rot);
        if (m_window.isKeyPressed(GLFW_KEY_UP))
            m_scene3d->m_camera->rotatePitch(rot);
        if (m_window.isKeyPressed(GLFW_KEY_DOWN))
            m_scene3d->m_camera->rotatePitch(-rot);
    }

    void init()
    {

        m_enablePostprocessing = false;

        s2d::Layer* layer = new s2d::Layer("HUD");
        layer->m_applyCamera = false;
        m_scene2d->m_layerStack->addLayer(layer);

        fps->m_color = glm::vec3(1.0f);
        m_scene2d->addObject(fps, "HUD");

        // add shader
        m_resourceManager.m_shaderManager.addShader(
            "model_shader_test",
            "res/shaders/test.vs",
            "res/shaders/test.fs"
        );

        // setup model shader
        graphics::Shader* shader = m_resourceManager.m_shaderManager.getShader("model_shader_test");
        shader->use();
        shader->setTexture("u_Texture");

        // load model
        s3d::Model* model = new s3d::Model("assets/models/tower-round-top-a.fbx");
        m_scene3d->addModel(model);
    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
