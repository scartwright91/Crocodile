#include "BatchRenderer.h"

namespace Crocodile
{
    namespace s2d
    {
        BatchRenderer::BatchRenderer(
            graphics::Shader *shader,
            std::vector<glm::vec2> positions,
            glm::vec2 size) : shader(shader), positions(positions), size(size)
        {
            init();
            initShader();
        }

        BatchRenderer::~BatchRenderer()
        {
            glDeleteVertexArrays(1, &this->VAO);
        }

        void BatchRenderer::init()
        {
            // configure VAO/VBO
            unsigned int VBO;

            std::vector<float> vertices = {};

            for (glm::vec2 pos : positions)
            {
                // v1
                vertices.push_back(pos.x);
                vertices.push_back(pos.y + size.y);
                vertices.push_back(0.f);
                vertices.push_back(1.f);
                // v2
                vertices.push_back(pos.x + size.x);
                vertices.push_back(pos.y);
                vertices.push_back(1.f);
                vertices.push_back(0.f);
                // v3
                vertices.push_back(pos.x);
                vertices.push_back(pos.y);
                vertices.push_back(0.f);
                vertices.push_back(0.f);
                // v4
                vertices.push_back(pos.x);
                vertices.push_back(pos.y + size.y);
                vertices.push_back(0.f);
                vertices.push_back(1.f);
                // v5
                vertices.push_back(pos.x + size.x);
                vertices.push_back(pos.y + size.y);
                vertices.push_back(1.f);
                vertices.push_back(1.f);
                // v6
                vertices.push_back(pos.x + size.x);
                vertices.push_back(pos.y);
                vertices.push_back(1.f);
                vertices.push_back(0.f);
            }

            glGenVertexArrays(1, &this->VAO);
            glGenBuffers(1, &VBO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

            glBindVertexArray(this->VAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void BatchRenderer::render(
            glm::mat4 view,
            glm::mat4 projection,
            unsigned int textureID,
            float alpha)
        {
            // prepare shader
            shader->use();
            shader->setMat4("u_Model", glm::mat4(1.f));
            shader->setMat4("u_View", view);
            shader->setMat4("u_Projection", projection);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureID);
            shader->setFloat("u_Alpha", alpha);
            // bind and draw
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6 * positions.size());
            glBindVertexArray(0);
        }

        void BatchRenderer::initShader()
        {
            this->shader->use();
            this->shader->setInt("u_Texture", 0);
        }
    }
}