#pragma once

#include <glm/glm.hpp>

#include "Renderer.h"
#include "../../graphics/Shader.h"

namespace Crocodile
{
    namespace s2d
    {
        class CROCODILE_API CircleRenderer : public Renderer
        {

        public:
            CircleRenderer(graphics::Shader *shader);

            void render(
                float thickness,
                float fade,
                glm::mat4 model,
                glm::mat4 view,
                glm::mat4 projection,
                glm::vec3 color,
                float alpha,
                float layerAlpha);

        private:
            void initShader();
        };
    }
}
