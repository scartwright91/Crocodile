#include "Scene.h"

Scene::Scene(graphics::Window *window, ResourceManager *resourceManager) : s3d::Scene(window, resourceManager)
{
    reflectionFB = new graphics::FrameBuffer(window->getWidth() / 2, window->getHeight() / 2);
    refractionFB = new graphics::FrameBuffer(window->getWidth() / 2, window->getHeight() / 2);
}

Scene::~Scene()
{
    delete reflectionFB;
    delete refractionFB;
}

void Scene::render()
{
    glm::mat4 model = glm::mat4(1.f);

    // reflection pass
    glViewport(0, 0, reflectionFB->width, reflectionFB->height);
    reflectionFB->bind();
    glm::vec3 camPos = camera->getPosition();
    float d = 2 * camPos.y;
    camPos.y -= d;
    camera->setPosition(camPos);
    //camera->invertPitch();
    for (EarthSurface* surf : earthSurfaces)
    {
        surf->customRender(
            model,
            camera->getViewMatrix(),
            camera->getProjectionMatrix(),
            camera->getPosition(),
            ambientLighting,
            lightPosition,
            lightColour,
            resourceManager->getTexture("earth_texture"),
            glm::vec4(0, 1, 0, 0)
        );
    }
    camPos.y += d;
    camera->setPosition(camPos);
    //camera->invertPitch();
    reflectionFB->unbind();

    // refraction pass
    glViewport(0, 0, refractionFB->width, refractionFB->height);
    refractionFB->bind();
    for (EarthSurface* surf : earthSurfaces)
    {
        surf->customRender(
            model,
            camera->getViewMatrix(),
            camera->getProjectionMatrix(),
            camera->getPosition(),
            ambientLighting,
            lightPosition,
            lightColour,
            resourceManager->getTexture("earth_texture"),
            glm::vec4(0, -1, 0, 0)
        );
    }
    refractionFB->unbind();

    glViewport(0, 0, window->getWidth(), window->getHeight());
    for (EarthSurface* surf : earthSurfaces)
    {
        surf->customRender(
            model,
            camera->getViewMatrix(),
            camera->getProjectionMatrix(),
            camera->getPosition(),
            ambientLighting,
            lightPosition,
            lightColour,
            resourceManager->getTexture("earth_texture"),
            glm::vec4(0, -1, 0, 1000)
        );
    }

    // render water last
    for (WaterSurface* surf : waterSurfaces)
    {
        surf->customRender(
            model,
            camera->getViewMatrix(),
            camera->getProjectionMatrix(),
            reflectionFB->textureColorbuffer,
            refractionFB->textureColorbuffer,
            resourceManager->getTexture("distortion_texture").textureID,
            timer,
            camera->getPosition()
        );
    }
}

void Scene::updateTimer(float dt)
{
    timer += 0.01f * dt;
    if (timer > 1.0)
        timer = 0.0f;
}
