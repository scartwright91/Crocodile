#include "Scene.h"

Scene::Scene(graphics::Window *window, ResourceManager *resourceManager) : s3d::Scene(window, resourceManager)
{

}

Scene::~Scene()
{

}

void Scene::render()
{

    for (s3d::Surface* surf : surfaces)
        surf->render(
            glm::mat4(1.f),
            camera->getViewMatrix(),
            camera->getProjectionMatrix(),
            camera->position,
            ambientLighting,
            lightPosition,
            lightColour
        );
    
    for (WaterSurface* surf : waterSurfaces)
    {
        surf->customRender();
    }
}
