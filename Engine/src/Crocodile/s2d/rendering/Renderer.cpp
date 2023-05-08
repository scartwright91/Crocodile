#include "Renderer.h"

namespace Crocodile
{
    namespace s2d
    {
        Renderer::Renderer(graphics::Shader *shader)
        {
            init();
            this->shader = shader;
        }

        Renderer::~Renderer()
        {
            glDeleteVertexArrays(1, &this->VAO);
        }

        void Renderer::render()
        {
        }

        void Renderer::init()
        {
            // configure VAO/VBO
            unsigned int VBO;
            float vertices[] = {
                // pos      // tex
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,

                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f};

            glGenVertexArrays(1, &this->VAO);
            glGenBuffers(1, &VBO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(this->VAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void Renderer::initShader()
        {
        }

    }
}