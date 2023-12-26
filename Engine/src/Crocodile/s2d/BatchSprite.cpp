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
            float tilesetHeight) : positions(positions), tilesetPositions(tilesetPositions), tilesetWidth(tilesetWidth), tilesetHeight(tilesetHeight)
        {
            renderMethod = "batch_sprite";
            this->size = size;
            createRenderer(shader);
        }

        BatchSprite::~BatchSprite()
        {
            delete renderer;
        }

        void BatchSprite::render(
            glm::vec2 viewportScale,
            glm::mat4 view,
            glm::mat4 projection,
            ResourceManager::TextureData texture,
            float alpha,
            float ambientLighting,
            std::vector<Light *> lights
            )
        {
            renderer->render(viewportScale, view, projection, texture, alpha, ambientLighting, lights);
        }

        void BatchSprite::createRenderer(graphics::Shader *shader)
        {
            renderer = new BatchRenderer(
                shader,
                positions,
                tilesetPositions,
                size,
                tilesetWidth,
                tilesetHeight);
        }

    }
}