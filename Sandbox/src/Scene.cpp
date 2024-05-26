#include "Scene.h"

Scene::Scene(graphics::Window *window, ResourceManager *resourceManager) : s3d::Scene(window, resourceManager)
{
    reflectionFB = new graphics::FrameBuffer(window->getWidth() / 8, window->getHeight() / 8);
    refractionFB = new graphics::FrameBuffer(window->getWidth() / 8, window->getHeight() / 8);
}

Scene::~Scene()
{
    delete reflectionFB;
    delete refractionFB;
}

void Scene::render()
{

    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 proj = camera->getProjectionMatrix();

    // reflection pass
    glViewport(0, 0, reflectionFB->width, reflectionFB->height);
    reflectionFB->bind();
    for (EarthSurface* surf : earthSurfaces)
    {
        surf->customRender(
            model,
            view,
            proj,
            camera->position,
            ambientLighting,
            lightPosition,
            lightColour,
            resourceManager->getTexture("earth_texture"),
            glm::vec4(0, -1, 0, 0)
        );
    }
    reflectionFB->unbind();

    // refraction pass
    glViewport(0, 0, refractionFB->width, refractionFB->height);
    refractionFB->bind();
    for (EarthSurface* surf : earthSurfaces)
    {
        surf->customRender(
            model,
            view,
            proj,
            camera->position,
            ambientLighting,
            lightPosition,
            lightColour,
            resourceManager->getTexture("earth_texture"),
            glm::vec4(0, 1, 0, 0)
        );
    }
    refractionFB->unbind();

    glViewport(0, 0, window->getWidth(), window->getHeight());
    for (EarthSurface* surf : earthSurfaces)
    {
        surf->customRender(
            model,
            view,
            proj,
            camera->position,
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
            view,
            proj
        );
    }
}
