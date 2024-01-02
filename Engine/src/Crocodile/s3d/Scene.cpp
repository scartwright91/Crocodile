#include "Scene.h"

namespace Crocodile
{
    namespace s3d
    {
        Scene::Scene(graphics::Window *window, ResourceManager *resourceManager) : window(window), resourceManager(resourceManager)
        {
            init();
            camera = new Camera();
        };

        Scene::~Scene()
        {
            delete camera;
        };

        void Scene::update(float dt)
        {
            camera->update(dt);
        }

        void Scene::render()
        {
            for (Surface* surf : surfaces)
                surf->render(
                    glm::mat4(1.f),
                    camera->getViewMatrix(),
                    camera->getProjectionMatrix()
                );
            for (Object* obj : objects)
                renderer->render(
                    obj->calculateModelMatrix(),
                    camera->getViewMatrix(),
                    camera->getProjectionMatrix()
                );
        };

        void Scene::addObject(Object* obj)
        {
            objects.push_back(obj);
        }

        void Scene::removeObject(Object* obj)
        {
            //TODO - remove object from vector
        }

        void Scene::init()
        {
            renderer = new Renderer(resourceManager->getShader("shader"));
        }
    }
}