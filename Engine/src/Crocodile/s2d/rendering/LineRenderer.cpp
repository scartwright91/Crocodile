#include "LineRenderer.h"

namespace Crocodile
{
    namespace s2d
    {
        LineRenderer::LineRenderer(graphics::Shader *shader) : shader(shader)
        {
            initShader();
        }

        LineRenderer::~LineRenderer()
        {
            glDeleteVertexArrays(1, &this->VAO);
        }

        void LineRenderer::render(
            glm::vec2 p1,
            glm::vec2 p2,
            glm::mat4 view,
            glm::mat4 projection,
            glm::vec3 color,
            float alpha)
        {
            init(p1, p2);
            // prepare shader
            shader->use();
            shader->setMat4("u_Model", glm::mat4(1.f));
            shader->setMat4("u_View", view);
            shader->setMat4("u_Projection", projection);
            shader->setVec3("u_Color", color);
            shader->setFloat("u_Alpha", alpha);
            // bind and draw
            glBindVertexArray(VAO);
            glDrawArrays(GL_LINES, 0, 2);
            glBindVertexArray(0);
        }

        void LineRenderer::init(glm::vec2 p1, glm::vec2 p2)
        {
            unsigned int VBO;
            float vertices[] = {
                p1.x,
                p1.y,
                p2.x,
                p2.y,
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void LineRenderer::initShader()
        {
            this->shader->use();
        }

    }
}