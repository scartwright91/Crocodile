
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <lua.hpp>

#include "Crocodile.h"
#include "Crocodile/utils/Logger.h"
#include "Crocodile/s3d/Surface.h"

#include "src/Scene.h"

using namespace Crocodile;


class Sandbox : public Crocodile::Application
{

public:

    float timer = 0.0f;
    float elapsed = 0.0f;
    float rotateSpeed = 1000.f;
    int currentScroll = 0;

    glm::vec2 clickPosition = {0.f, 0.f};
    bool mouseDown = false;

    Scene* scene = nullptr;
    s2d::Text* fps = new s2d::Text();
    EarthSurface* earthSurface = nullptr;
    WaterSurface* waterSurface = nullptr;
    s3d::Object* light = nullptr;

    Sandbox() : Crocodile::Application("Sandbox", false, 1280, 720, false)
    {
        init();

        // Create a new Lua state
        lua_State* L = luaL_newstate();

        // Load the standard libraries
        luaL_openlibs(L);

        // Load and run a Lua script
        int status = luaL_loadfile(L, "res/scripts/script.lua");
        if (status == 0) {
            // Execute the script
            status = lua_pcall(L, 0, 0, 0);
        }

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

        // rotate light
        // glm::vec3 p = glm::vec3(
        //     earthSurface->heightMap.nCols / 2,
        //     earthSurface->heightMap.maxHeight * 10.0,
        //     earthSurface->heightMap.nRows / 2
        // );
        // glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(elapsed), {0.f, 1.f, 0.f});
        // scene3d->m_lightPosition = glm::vec3(glm::vec4(p, 1.0f) * rot_mat);

        processCommands(dt);

        if (window.isKeyPressed(GLFW_KEY_SPACE) && (elapsed > 1.0f))
        {
            elapsed = 0.0f;
            if (scene3d->m_renderer->getRenderMode() == s3d::FILL)
                scene3d->m_renderer->setRenderMode(s3d::LINE);
            else
                scene3d->m_renderer->setRenderMode(s3d::FILL);
        }

        scene->updateTimer(dt);

    }


    void processCommands(float dt)
    {
        // moving camera
        if (window.isKeyPressed(GLFW_KEY_A))
            scene3d->m_camera->move(s3d::LEFT, dt);
        if (window.isKeyPressed(GLFW_KEY_D))
            scene3d->m_camera->move(s3d::RIGHT, dt);
        if (window.isKeyPressed(GLFW_KEY_W))
            scene3d->m_camera->move(s3d::FORWARD, dt);
        if (window.isKeyPressed(GLFW_KEY_S))
            scene3d->m_camera->move(s3d::BACKWARD, dt);

        // rotating camera
        float rot = 100.f * dt;
        if (window.isKeyPressed(GLFW_KEY_LEFT))
            scene->m_camera->rotateYaw(rot);
        if (window.isKeyPressed(GLFW_KEY_RIGHT))
            scene->m_camera->rotateYaw(-rot);
        if (window.isKeyPressed(GLFW_KEY_UP))
            scene->m_camera->rotatePitch(rot);
        if (window.isKeyPressed(GLFW_KEY_DOWN))
            scene->m_camera->rotatePitch(-rot);
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

    void createEarthSurface()
    {
        // height maps
        s3d::HeightMap telemetryHeightMap("res/textures/earth_topography_high_res.png", 20.f, false);
        s3d::HeightMap bathymetrycHeightMap("res/textures/earth_bathymetry_high_res.png", -30.f, true);
        s3d::HeightMap earthHeightMap(
            telemetryHeightMap.nCols,
            telemetryHeightMap.nRows,
            calculateEarthHeightMap(telemetryHeightMap, bathymetrycHeightMap)
        );

        // add shader
        resourceManager.shaderManager.addShader("earth_surface", "res/shaders/earth_shader.vs", "res/shaders/earth_shader.fs");
        
        // create earth surface
        earthSurface = new EarthSurface(earthHeightMap, resourceManager.shaderManager.getShader("earth_surface"));
        earthSurface->adjacentVertexDistance = 10;
        earthSurface->createSurface();
        scene->m_earthSurfaces.push_back(earthSurface);
    }

    void createWaterSurface()
    {
        // add shader
        resourceManager.shaderManager.addShader("water_surface", "res/shaders/water_shader.vs", "res/shaders/water_shader.fs");

        // create water surface
        float h = 0.f;
        std::vector<float> heights = {};
        int nCols = earthSurface->heightMap.nCols / 100 + 1;
        int nRows = earthSurface->heightMap.nRows / 100 + 1;
        for (int i = 0; i < nCols; i++)
            for (int j = 0; j < nRows; j++)
                heights.push_back(0.f);
        s3d::HeightMap waterHeightMap(nCols, nRows, heights);
        waterSurface = new WaterSurface(&window, waterHeightMap, resourceManager.shaderManager.getShader("water_surface"));
        waterSurface->colour = glm::vec3(0.f, 0.f, 1.f);
        waterSurface->gridSize = {100, 100};
        waterSurface->createSurface();
        scene->m_waterSurfaces.push_back(waterSurface);

    }
    
    void init()
    {

        window.setBackgroundColor(glm::vec3(0.02f, 0.13f, 0.22f));
        postProcessing->setPostProcessingEffect(graphics::GREYSCALE);
        enablePostprocessing = false;

        // create new scene and set it as current scene
        scene = new Scene(&window, &resourceManager);
        setCurrentScene3d(scene);

        resourceManager.loadTexture("res/textures/earth_texture_oct_medium.png", "earth_texture", false);

        s2d::Layer* layer = new s2d::Layer("hud");
        layer->applyCamera = false;
        scene2d->layerStack->addLayer(layer);

        fps->color = glm::vec3(1.f);
        scene2d->addObject(fps, "hud");

        createEarthSurface();
        createWaterSurface();

        // create light cube
        light = new s3d::Object(
            glm::vec3(
                earthSurface->heightMap.nCols / 2,
                earthSurface->heightMap.maxHeight * 10.0,
                earthSurface->heightMap.nRows / 4
            )
        );
        light->m_size = glm::vec3(50.f);
        light->m_colour = glm::vec3(1.f);
        scene3d->addObject(light);

        /*
        // debug reflection pass
        resourceManager.addTexture(
            scene->m_reflectionFB->textureColorbuffer,
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
        */

        glm::vec3 p = glm::vec3(
            earthSurface->heightMap.nCols / 2,
            earthSurface->heightMap.maxHeight * 5.0,
            earthSurface->heightMap.nRows / 2
        );
        scene3d->m_camera->setPosition(p);

        scene3d->m_lightPosition = light->m_position;
        scene3d->m_ambientLighting = 0.2f;

    }

};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
