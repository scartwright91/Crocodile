#pragma once

#include "../Core.h"

#include "Object.h"
#include "rendering/BatchRenderer.h"
#include "../ResourceManager.h"

namespace Crocodile
{
    namespace s2d
    {
        class CROCODILE_API BatchSprite : public Object
        {
        public:
            BatchSprite(graphics::Shader *shader, std::vector<glm::vec2> positions, glm::vec2 size);
            ~BatchSprite();

            void render(
                glm::mat4 view,
                glm::mat4 projection,
                unsigned int textureID,
                float alpha);

            void createRenderer(graphics::Shader *shader);

            BatchRenderer *renderer = nullptr;
            std::vector<glm::vec2> positions = {};
        };
    }
}