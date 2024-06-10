#include "Scene.h"

Scene::Scene(graphics::Window *window, ResourceManager *resourceManager) : s3d::Scene(window, resourceManager)
{
    m_reflectionFB = new graphics::FrameBuffer(window->getWidth() / 2, window->getHeight() / 2);
    m_refractionFB = new graphics::FrameBuffer(window->getWidth() / 2, window->getHeight() / 2);
}

Scene::~Scene()
{
    delete m_reflectionFB;
    delete m_refractionFB;
}

void Scene::render()
{
    glm::mat4 model = glm::mat4(1.f);

    // reflection pass
    glViewport(0, 0, m_reflectionFB->width, m_reflectionFB->height);
    m_reflectionFB->bind();
    glm::vec3 camPos = m_camera->getPosition();
    float d = 2 * camPos.y;
    camPos.y -= d;
    m_camera->setPosition(camPos);
    m_camera->invertPitch();
    for (EarthSurface* surf : m_earthSurfaces)
    {
        surf->customRender(
            model,
            m_camera->getViewMatrix(),
            m_camera->getProjectionMatrix(),
            m_camera->getPosition(),
            m_ambientLighting,
            m_lightPosition,
            m_lightColour,
            m_resourceManager->getTexture("earth_texture"),
            glm::vec4(0, 1, 0, 0)
        );
    }
    camPos.y += d;
    m_camera->setPosition(camPos);
    m_camera->invertPitch();
    m_reflectionFB->unbind();

    // refraction pass
    glViewport(0, 0, m_refractionFB->width, m_refractionFB->height);
    m_refractionFB->bind();
    for (EarthSurface* surf : m_earthSurfaces)
    {
        surf->customRender(
            model,
            m_camera->getViewMatrix(),
            m_camera->getProjectionMatrix(),
            m_camera->getPosition(),
            m_ambientLighting,
            m_lightPosition,
            m_lightColour,
            m_resourceManager->getTexture("earth_texture"),
            glm::vec4(0, -1, 0, 0)
        );
    }
    m_refractionFB->unbind();

    glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
    for (EarthSurface* surf : m_earthSurfaces)
    {
        surf->customRender(
            model,
            m_camera->getViewMatrix(),
            m_camera->getProjectionMatrix(),
            m_camera->getPosition(),
            m_ambientLighting,
            m_lightPosition,
            m_lightColour,
            m_resourceManager->getTexture("earth_texture"),
            glm::vec4(0, -1, 0, 1000)
        );
    }

    // render water last
    for (WaterSurface* surf : m_waterSurfaces)
    {
        surf->customRender(
            model,
            m_camera->getViewMatrix(),
            m_camera->getProjectionMatrix(),
            m_reflectionFB->textureColorbuffer,
            m_refractionFB->textureColorbuffer,
            m_resourceManager->getTexture("dudv").textureID,
            m_resourceManager->getTexture("normal_map").textureID,
            m_timer,
            m_camera->getPosition(),
            m_lightPosition,
            m_lightColour
        );
    }

    for (s3d::Object* obj : m_objects)
        m_renderer->render(
            obj->calculateModelMatrix(),
            m_camera->getViewMatrix(),
            m_camera->getProjectionMatrix(),
            obj->m_colour
        );

}

void Scene::updateTimer(float dt)
{
    m_timer += 0.01f * dt;
    if (m_timer > 1.0)
        m_timer = 0.0f;
}
