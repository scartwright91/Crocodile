#pragma once

#include "../../Core.h"
#include "../../graphics/Shader.h"

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
                glm::vec2 size);
            ~BatchRenderer();

            void render(
                glm::mat4 view,
                glm::mat4 projection,
                unsigned int textureID,
                float alpha);
            void initShader();

            graphics::Shader *shader;
            unsigned int VAO = 0;

        private:
            void init();
            std::vector<glm::vec2> positions;
            glm::vec2 size;
        };
    }
}
