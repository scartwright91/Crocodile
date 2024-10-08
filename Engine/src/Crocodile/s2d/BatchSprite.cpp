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
            TextureData tileset) : m_positions(positions), m_tilesetPositions(tilesetPositions)
        {
            m_renderMethod = "batch_sprite";
            setSize(size);
            setTexture(tileset);
            createRenderer(shader);
        }

        BatchSprite::BatchSprite(
            graphics::Shader *shader,
            std::vector<glm::vec2> positions,
            glm::vec2 size,
            glm::vec3 colour) : m_positions(positions)
        {
            m_renderMethod = "batch_sprite";
            m_color = colour;
            setSize(size);
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
            m_renderer->render(
                viewportScale,
                view,
                projection,
                texture,
                m_color,
                alpha,
                ambientLighting,
                lights
            );
        }

        void BatchSprite::createRenderer(graphics::Shader *shader)
        {
            if (m_useTexture)
            {
                m_renderer = new BatchRenderer(
                    shader,
                    m_positions,
                    m_tilesetPositions,
                    getSize(),
                    m_texture.width,
                    m_texture.height);
            }
            else
            {
                m_renderer = new BatchRenderer(
                    shader,
                    m_positions,
                    getSize()
                );
            }
        }

    }
}