#pragma once

#include <glm/glm.hpp>

#include "Renderer.h"
#include "../LightSystem.h"

namespace Crocodile
{
    namespace s2d
    {
        class CROCODILE_API LineRenderer : public Renderer
        {

        public:
            LineRenderer(graphics::Shader *shader);

            void render();

        private:
            void initShader();
        };
    }
}
