#include "BatchSprite.h"

namespace Crocodile
{
    namespace s2d
    {

        BatchSprite::BatchSprite(graphics::Shader *shader, std::vector<glm::vec2> positions, glm::vec2 size) : positions(positions)
        {
            renderMethod = "batch_sprite";
            this->size = size;
            createRenderer(shader);
        }

        BatchSprite::~BatchSprite()
        {
            delete renderer;
        }

        void BatchSprite::render(glm::mat4 view, glm::mat4 projection, unsigned int textureID, float alpha)
        {
            renderer->render(view, projection, textureID, alpha);
        }

        void BatchSprite::createRenderer(graphics::Shader *shader)
        {
            renderer = new BatchRenderer(shader, positions, size);
        }

    }
}