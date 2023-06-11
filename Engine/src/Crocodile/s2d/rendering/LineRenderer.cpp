#include "LineRenderer.h"

namespace Crocodile
{
    namespace s2d
    {
        LineRenderer::LineRenderer(graphics::Shader *shader) : Renderer(shader)
        {
            initShader();
        }

        void LineRenderer::render()
        {
            // prepare shader
            shader->use();
            // bind and draw
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        void LineRenderer::initShader()
        {
            this->shader->use();
        }

    }
}