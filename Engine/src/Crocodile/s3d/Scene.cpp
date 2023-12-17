#include "Scene.h"

namespace Crocodile
{
    namespace s3d
    {
        Scene::Scene(graphics::Window *window, ResourceManager *resourceManager) : window(window), resourceManager(resourceManager)
        {
            init();
        };

        Scene::~Scene()
        {

        };

        void Scene::update(float dt)
        {
            
        }

        void Scene::render()
        {
            for (Object* obj : objects)
                renderer->render();
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