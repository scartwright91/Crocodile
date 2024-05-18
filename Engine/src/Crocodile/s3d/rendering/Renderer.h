#pragma once

#include "../../Core.h"
#include "../../graphics/Shader.h"

namespace Crocodile
{
    namespace s3d
    {
        
        enum RenderMode
        {
            LINE,
            FILL
        };

        class CROCODILE_API Renderer
        {

            public:
                Renderer(graphics::Shader *shader);
                ~Renderer();

                void render(
                    glm::mat4 model,
                    glm::mat4 view,
                    glm::mat4 projection
                );
                void setRenderMode(RenderMode mode);
                RenderMode getRenderMode();

                graphics::Shader *shader;
                unsigned int VAO = 0;

            private:
                void init();
                RenderMode renderMode = FILL;

        };
    }
}
