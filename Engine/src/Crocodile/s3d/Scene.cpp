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

        };

        void Scene::update(float dt)
        {
            camera->update(dt);
        }

        void Scene::render()
        {
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