#include "BatchSprite.h"

namespace Crocodile
{
    namespace s2d
    {

        BatchSprite::BatchSprite(
            graphics::Shader *shader,
            std::vector<glm::vec2> positions,
            std::vector<glm::vec2> tilesetPositions,
            glm::vec2 size,
            float tilesetWidth,
            float tilesetHeight) : m_positions(positions), m_tilesetPositions(tilesetPositions), m_tilesetWidth(tilesetWidth), m_tilesetHeight(tilesetHeight)
        {
            m_renderMethod = "batch_sprite";
            m_size = size;
            createRenderer(shader);
        }

        BatchSprite::~BatchSprite()
        {
            delete m_renderer;
        }

        void BatchSprite::render(
            glm::vec2 viewportScale,
            glm::mat4 view,
            glm::mat4 projection,
            TextureData texture,
            float alpha,
            float ambientLighting,
            std::vector<Light *> lights
            )
        {
            m_renderer->render(viewportScale, view, projection, texture, alpha, ambientLighting, lights);
        }

        void BatchSprite::createRenderer(graphics::Shader *shader)
        {
            m_renderer = new BatchRenderer(
                shader,
                m_positions,
                m_tilesetPositions,
                m_size,
                m_tilesetWidth,
                m_tilesetHeight);
        }

    }
}