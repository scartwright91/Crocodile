#pragma once

#include "../../Core.h"
#include "../../graphics/Shader.h"

#include "../../ResourceManager.h"
#include "../LightSystem.h"

namespace Crocodile
{
    namespace s2d
    {
        class CROCODILE_API BatchRenderer
        {
        public:
            BatchRenderer(
                graphics::Shader *shader,
                std::vector<glm::vec2> positions,
                std::vector<glm::vec2> tilesetPositions,
                glm::vec2 size,
                float tilesetWidth,
                float tilesetHeight);
            ~BatchRenderer();

            void render(
                glm::vec2 viewportScale,
                glm::mat4 view,
                glm::mat4 projection,
                TextureData texture,
                float alpha,
                float ambientLighting,
                std::vector<Light *> lights);
            void initShader();

            graphics::Shader *shader;
            unsigned int VAO = 0;

        private:
            void init();
            std::vector<glm::vec2> positions;
            std::vector<glm::vec2> tilesetPositions;
            glm::vec2 size;
            float tilesetWidth;
            float tilesetHeight;
        };
    }
}
