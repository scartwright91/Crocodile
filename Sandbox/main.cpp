
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "Crocodile.h"
#include "Crocodile/s3d/Surface.h"

#include "src/Scene.h"

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:

    float timer = 0.0f;
    float elapsed = 0.0f;
    float rotateSpeed = 1000.f;

    Scene* scene = nullptr;
    s2d::Text* fps = new s2d::Text();
    EarthSurface* earthSurface = nullptr;
    WaterSurface* waterSurface = nullptr;

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
        postProcessing->setPostProcessingEffect(graphics::GREYSCALE);
        enablePostprocessing = false;

        // create new scene and set it as current scene
        scene = new Scene(&window, &resourceManager);
        setCurrentScene3d(scene);
        scene->camera->frustrumMax = 100000.f;

        resourceManager.loadTexture("res/textures/earth_texture_oct_medium.png", "earth_texture", false);

        s2d::Layer* layer = new s2d::Layer("hud");
        layer->applyCamera = false;
        scene2d->layerStack->addLayer(layer);

        fps->color = glm::vec3(1.f);
        scene2d->addObject(fps, "hud");

        createEarthSurface();
        createWaterSurface();

        // debug reflection pass
        resourceManager.addTexture(
            scene->reflectionFB->textureColorbuffer,
            window.getWidth() / 4,
            window.getHeight() / 4,
            "reflection"
        );
        s2d::Object* t = new s2d::Object();
        t->size = glm::vec2(window.getWidth() / 4, window.getHeight() / 4);
        t->setTexture(resourceManager.getTexture("reflection"));
        scene2d->addObject(t, "hud");

        // debug refraction pass
        resourceManager.addTexture(
            scene->refractionFB->textureColorbuffer,
            window.getWidth() / 4,
            window.getHeight() / 4,
            "refraction"
        );
        s2d::Object* t2 = new s2d::Object();
        t2->setPosition(glm::vec2(0.75 * window.getWidth(), 0));
        t2->size = glm::vec2(window.getWidth() / 4, window.getHeight() / 4);
        t2->setTexture(resourceManager.getTexture("refraction"));
        scene2d->addObject(t2, "hud");

        scene3d->camera->position = glm::vec3(
            earthSurface->heightMap.nCols / 2,
            earthSurface->heightMap.maxHeight * 1.0,
            earthSurface->heightMap.nRows / 2
        );
        scene3d->lightPosition = glm::vec3(
            earthSurface->heightMap.nCols / 2,
            earthSurface->heightMap.maxHeight * 10.0,
            0
        );

        scene3d->camera->Speed = 2000.0f;
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
            xoffset = -rotateSpeed * dt;
        if (window.isKeyPressed(GLFW_KEY_RIGHT))
            xoffset = rotateSpeed * dt;

        if (xoffset == 0.0)
        {
            if (window.isKeyPressed(GLFW_KEY_UP))
                yoffset = -rotateSpeed * dt;
            if (window.isKeyPressed(GLFW_KEY_DOWN))
                yoffset = rotateSpeed * dt;
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
                    heights.push_back(h1 + 1.f);
                else
                    heights.push_back(h2 - 1.f);
            }
        }
        return heights;
    }

    std::vector<float> smoothHeightMap(s3d::HeightMap heightMap)
    {
        std::vector<float> heights = {};
        for(int i = 0; i < heightMap.nRows; i++)
        {
            for(int j = 0; j < heightMap.nCols; j++)
            {
                if ((i == 0) || (i == heightMap.nRows - 1) || (j == 0) || (j == heightMap.nCols - 1))
                    heights.push_back(heightMap.getHeight(i, j));
                else
                {
                    float h = (
                        heightMap.getHeight(i, j) +
                        heightMap.getHeight(i - 1, j) +
                        heightMap.getHeight(i + 1, j) +
                        heightMap.getHeight(i, j - 1) +
                        heightMap.getHeight(i, j + 1)
                    ) / 5;
                    heights.push_back(h);
                }
            }
        }
        return heights;
    }

    void createEarthSurface()
    {
        // height maps
        s3d::HeightMap telemetryHeightMap("res/textures/earth_topography_high_res.png", 100.f, false);
        telemetryHeightMap.heights = smoothHeightMap(telemetryHeightMap);
        telemetryHeightMap.heights = smoothHeightMap(telemetryHeightMap);
        telemetryHeightMap.heights = smoothHeightMap(telemetryHeightMap);
        s3d::HeightMap bathymetrycHeightMap("res/textures/earth_bathymetry_high_res.png", -150.f, true);
        bathymetrycHeightMap.heights = smoothHeightMap(bathymetrycHeightMap);
        bathymetrycHeightMap.heights = smoothHeightMap(bathymetrycHeightMap);
        bathymetrycHeightMap.heights = smoothHeightMap(bathymetrycHeightMap);
        s3d::HeightMap earthHeightMap(
            telemetryHeightMap.nCols,
            telemetryHeightMap.nRows,
            calculateEarthHeightMap(telemetryHeightMap, bathymetrycHeightMap)
        );

        graphics::Shader* earthShader = new graphics::Shader(
            "res/shaders/earth_shader.vs",
            "res/shaders/earth_shader.fs"
        );
        
        // create earth surface
        earthSurface = new EarthSurface(earthHeightMap, earthShader);
        earthSurface->adjacentVertexDistance = 10;
        earthSurface->createSurface();
        scene->earthSurfaces.push_back(earthSurface);
    }

    void createWaterSurface()
    {
        graphics::Shader* waterShader = new graphics::Shader(
            "res/shaders/water_shader.vs",
            "res/shaders/water_shader.fs"
        );

        // create water surface
        float h = 0.f;
        std::vector<float> heights = {};
        int nCols = earthSurface->heightMap.nCols / 100;
        int nRows = earthSurface->heightMap.nRows / 100;
        for (int i = 0; i < nCols; i++)
            for (int j = 0; j < nRows; j++)
                heights.push_back(0.f);
        s3d::HeightMap waterHeightMap(nCols, nRows, heights);
        waterSurface = new WaterSurface(&window, waterHeightMap, waterShader);
        waterSurface->colour = glm::vec3(0.f, 0.f, 1.f);
        waterSurface->gridSize = {100, 100};
        waterSurface->createSurface();
        scene->waterSurfaces.push_back(waterSurface);

    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
