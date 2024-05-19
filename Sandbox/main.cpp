
#include "Crocodile.h"
#include "Crocodile/s3d/Surface.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:

    float timer = 0.0f;
    float elapsed = 0.0f;
    float rotateSpeed = 10.f;

    // mouse
    float lastX = 0.0f;
    float lastY = 0.0f;

    s2d::Text* fps = new s2d::Text();
    s3d::Surface* earthSurface = nullptr;
    s3d::Surface* waterSurface = nullptr;

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

        processCommands(dt);

        // rotate light position
        // scene3d->lightPosition = glm::vec3(
        //     surface->nRows / 2 + 800 * glm::cos(elapsed),
        //     surface->maxHeight * 10.0,
        //     surface->nCols / 2 + 800 * glm::sin(elapsed)
        // );

        if (window.isKeyPressed(GLFW_KEY_SPACE) && (elapsed > 1.0f))
        {
            elapsed = 0.0f;
            if (scene3d->renderer->getRenderMode() == s3d::FILL)
                scene3d->renderer->setRenderMode(s3d::LINE);
            else
                scene3d->renderer->setRenderMode(s3d::FILL);
        }

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

        s3d::HeightMap telemetryHeightMap("res/medium_earth_topography.png", 10.f, false);
        s3d::HeightMap bathymetrycHeightMap("res/medium_earth_bathymetry.png", -10.f, true);
        s3d::HeightMap earthHeightMap(
            telemetryHeightMap.nCols,
            telemetryHeightMap.nRows,
            calculateEarthHeightMap(telemetryHeightMap, bathymetrycHeightMap)
        );

        earthSurface = new s3d::Surface(earthHeightMap, resourceManager.getShader("surface_shader"));
        earthSurface->adjacentVertexDistance = 4;
        earthSurface->createSurface();

        scene3d->surfaces.push_back(earthSurface);

        scene3d->camera->position = glm::vec3(0, earthSurface->heightMap.maxHeight * 2.0, 0);
        scene3d->lightPosition = glm::vec3(
            earthSurface->heightMap.nCols / 2,
            earthSurface->heightMap.maxHeight * 10.0,
            earthSurface->heightMap.nRows / 2
        );

        scene3d->camera->Speed = 200.0f;
        scene3d->ambientLighting = 0.2f;

    }

    void processCommands(float dt)
    {
        if (window.isKeyPressed(GLFW_KEY_A))
            scene3d->camera->processMovement(s3d::Camera::LEFT, dt);
        if (window.isKeyPressed(GLFW_KEY_D))
            scene3d->camera->processMovement(s3d::Camera::RIGHT, dt);
        if (window.isKeyPressed(GLFW_KEY_W))
            scene3d->camera->processMovement(s3d::Camera::FORWARD, dt);
        if (window.isKeyPressed(GLFW_KEY_S))
            scene3d->camera->processMovement(s3d::Camera::BACKWARD, dt);

        float xoffset = 0.0;
        float yoffset = 0.0;
        if (window.isKeyPressed(GLFW_KEY_LEFT))
            xoffset = -rotateSpeed;
        if (window.isKeyPressed(GLFW_KEY_RIGHT))
            xoffset = rotateSpeed;

        if (xoffset == 0.0)
        {
            if (window.isKeyPressed(GLFW_KEY_UP))
                yoffset = -rotateSpeed;
            if (window.isKeyPressed(GLFW_KEY_DOWN))
                yoffset = rotateSpeed;
        }

        scene3d->camera->processMouseMovement(xoffset, yoffset);
    }

    std::vector<float> calculateEarthHeightMap(s3d::HeightMap telemetry, s3d::HeightMap bathymetry)
    {
        std::vector<float> heights = {};
        for(int i = 0; i < telemetry.nRows; i++)
        {
            for(int j = 0; j < telemetry.nCols; j++)
            {
                float h1 = telemetry.getHeight(i, j);
                float h2 = bathymetry.getHeight(i, j);
                if (h1 != 0.0)
                    heights.push_back(h1);
                else
                    heights.push_back(h2);
            }
        }
        return heights;
    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
