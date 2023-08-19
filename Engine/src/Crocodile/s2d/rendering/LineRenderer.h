#pragma once

#include <glm/glm.hpp>

#include "../../graphics/Shader.h"

namespace Crocodile
{
    namespace s2d
    {
        class CROCODILE_API LineRenderer
        {

        public:
            LineRenderer(graphics::Shader *shader);
            ~LineRenderer();

            void render(
                glm::vec2 p1,
                glm::vec2 p2,
                glm::mat4 view,
                glm::mat4 projection,
                glm::vec3 color,
                float alpha,
                float layerAlpha);

            graphics::Shader *shader;
            unsigned int VAO = 0;
            unsigned int VBO = 0;

        private:
            void init();
            void updateBuffer(glm::vec2 p1, glm::vec2 p2);
            void initShader();
        };
    }
}
