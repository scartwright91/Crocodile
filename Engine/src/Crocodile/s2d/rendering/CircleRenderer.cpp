#include "CircleRenderer.h"

namespace Crocodile
{
    namespace s2d
    {
        CircleRenderer::CircleRenderer(graphics::Shader *shader) : Renderer(shader)
        {
            initShader();
        }

        void CircleRenderer::render(
            float radius,
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection,
            glm::vec3 color,
            float alpha,
            float layerAlpha)
        {
            // prepare shader
            shader->use();
            shader->setFloat("u_Radius", radius);
            shader->setMat4("u_Model", model);
            shader->setMat4("u_View", view);
            shader->setMat4("u_Projection", projection);
            shader->setVec3("u_Color", color);
            shader->setFloat("u_Alpha", alpha);
            shader->setFloat("u_LayerAlpha", layerAlpha);
            // bind and draw
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        void CircleRenderer::initShader()
        {
            this->shader->use();
        }

    }
}