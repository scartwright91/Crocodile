#include "Scene.h"

namespace Crocodile
{
    namespace s3d
    {
        Scene::Scene(graphics::Window *window, ResourceManager *resourceManager) : m_window(window), m_resourceManager(resourceManager)
        {
            init();
            m_camera = new CameraController(window->getWidth(), window->getHeight());
        };

        Scene::~Scene()
        {
            delete m_camera;
            delete m_renderer;
        };

        void Scene::update(float dt)
        {

        }

        void Scene::resize()
        {
            m_camera->setWindowDimensions(m_window->getWidth(), m_window->getHeight());
        }

        void Scene::render()
        {
            for (Surface* surf : m_surfaces)
                surf->render(
                    glm::mat4(1.f),
                    m_camera->getViewMatrix(),
                    m_camera->getProjectionMatrix(),
                    m_camera->getPosition(),
                    m_ambientLighting,
                    m_lightPosition,
                    m_lightColour
                );
            for (Object* obj : m_objects)
                m_renderer->render(
                    obj->calculateModelMatrix(),
                    m_camera->getViewMatrix(),
                    m_camera->getProjectionMatrix(),
                    obj->m_colour
                );
            for (Model* model : m_models)
                model->render(
                    glm::mat4(1.f),
                    m_camera->getViewMatrix(),
                    m_camera->getProjectionMatrix(),
                    m_camera->getPosition(),
                    m_lightPosition,
                    m_lightColour,
                    m_ambientLighting
                );
        };

        void Scene::addObject(Object* obj)
        {
            m_objects.push_back(obj);
        }

        void Scene::removeObject(Object* obj)
        {
            m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), obj), m_objects.end());
        }

        void Scene::addModel(Model* model)
        {
            m_models.push_back(model);
        }

        void Scene::removeModel(Model* model)
        {
            m_models.erase(std::remove(m_models.begin(), m_models.end(), model), m_models.end());
        }

        void Scene::init()
        {
            m_renderer = new Renderer(m_resourceManager->m_shaderManager.getShader("shader"));
        }
    }
}