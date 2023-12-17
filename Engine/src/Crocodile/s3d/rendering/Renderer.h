#pragma once

#include "../../Core.h"
#include "../../graphics/Shader.h"

namespace Crocodile
{
    namespace s3d
    {
        class CROCODILE_API Renderer
        {
            public:
                Renderer(graphics::Shader *shader);
                ~Renderer();

                void render();

                graphics::Shader *shader;
                unsigned int VAO = 0;

            private:
                void init();

        };
    }
}
