#include "CircleRenderer.h"

namespace Crocodile
{
    namespace s2d
    {
        CircleRenderer::CircleRenderer(graphics::Shader *shader) : shader(shader)
        {
            initShader();
            init();
        }

        CircleRenderer::~CircleRenderer()
        {
            glDeleteVertexArrays(1, &this->VAO);
        }

        void CircleRenderer::render(
            float radius,
            glm::mat4 view,
            glm::mat4 projection,
            glm::vec3 color,
            float alpha,
            float layerAlpha)
        {
            // prepare shader
            shader->use();
            shader->setMat4("u_Model", glm::mat4(1.f));
            shader->setMat4("u_View", view);
            shader->setMat4("u_Projection", projection);
            shader->setVec3("u_Color", color);
            shader->setFloat("u_Alpha", alpha);
            shader->setFloat("u_LayerAlpha", layerAlpha);
            // bind and draw
            glBindVertexArray(VAO);
            glDrawArrays(GL_LINES, 0, 2);
            glBindVertexArray(0);
        }

        // void LineRenderer::updateBuffer(glm::vec2 p1, glm::vec2 p2)
        // {
        //     GLfloat vertices[] = {
        //         p1.x,
        //         p1.y,
        //         p2.x,
        //         p2.y,
        //     };
        //     glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        // }

        void CircleRenderer::init()
        {
            GLfloat vertices[] = {
                0.f,
                0.f,
                1.f,
                1.f,
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

        void CircleRenderer::initShader()
        {
            this->shader->use();
        }

    }
}