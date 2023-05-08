#pragma once

#include "../../Core.h"
#include "../../graphics/Shader.h"

namespace Crocodile
{
    namespace s2d
    {
        class CROCODILE_API Renderer
        {
        public:
            Renderer(graphics::Shader *shader);
            ~Renderer();

            virtual void render();
            virtual void initShader();

            graphics::Shader *shader;
            unsigned int VAO = 0;

        private:
            void init();
        };
    }
}
