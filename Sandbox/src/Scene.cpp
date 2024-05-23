#include "Scene.h"

Scene::Scene(graphics::Window *window, ResourceManager *resourceManager) : s3d::Scene(window, resourceManager)
{

}

Scene::~Scene()
{

}

void Scene::render()
{

    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 proj = camera->getProjectionMatrix();

    for (s3d::Surface* surf : surfaces)
        surf->render(
            model,
            view,
            proj,
            camera->position,
            ambientLighting,
            lightPosition,
            lightColour
        );
    
    for (WaterSurface* surf : waterSurfaces)
    {
        surf->customRender(
            model,
            view,
            proj
        );
    }
    for (EarthSurface* surf : earthSurfaces)
    {
        surf->customRender(
            model,
            view,
            proj,
            camera->position,
            ambientLighting,
            lightPosition,
            lightColour
        );
    }
}
