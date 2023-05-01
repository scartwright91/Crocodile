#include "SpriteRenderer.h"


namespace Crocodile
{
	namespace s2d
	{
        SpriteRenderer::SpriteRenderer(graphics::Shader* shader)
        {
            init();
            this->shader = shader;
            this->shader->use();
            this->shader->setInt("u_Texture", 0);
            this->shader->setInt("u_DistortionTexture", 1);
        }

        SpriteRenderer::~SpriteRenderer()
        {
            glDeleteVertexArrays(1, &this->VAO);
        }

        void SpriteRenderer::render(
            float time,
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection,
            unsigned int textureID,
            bool useTexture,
            float numberOfRows,
            float numberOfCols,
            glm::vec2 textureOffset,
            glm::vec3 spriteColor,
            float alpha,
            float ambientLighting,
            std::vector<Light*> lights,
            bool outline,
            float aspectRatio,
            unsigned int distortionTexture,
            bool useDistortionTexture,
            bool scrollDistortionX,
            bool scrollDistortionY,
            float distortionSpeed,
            bool flipX,
            bool flipY
        )
        {
            // prepare shader
            shader->use();
            shader->setFloat("u_Time", time);
            // vertex uniforms
            shader->setMat4("u_Model", model);
            shader->setMat4("u_View", view);
            shader->setMat4("u_Projection", projection);
            shader->setBool("u_UseTexture", useTexture);
            shader->setBool("u_UseDistortion", useDistortionTexture);
            shader->setBool("u_ScrollDistortionX", scrollDistortionX);
            shader->setBool("u_ScrollDistortionY", scrollDistortionY);
            shader->setFloat("u_DistortionSpeed", distortionSpeed);
            shader->setBool("u_FlipX", flipX);
            shader->setBool("u_FlipY", flipY);
            // texture
            if (useTexture)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textureID);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, distortionTexture);
                shader->setFloat("u_NumberOfRows", numberOfRows);
                shader->setFloat("u_NumberOfCols", numberOfCols);
                shader->setVec2("u_TextureOffset", textureOffset);
            }
            shader->setVec3("u_SpriteColor", spriteColor);
            shader->setFloat("u_Alpha", alpha);
            // lighting
            shader->setBool("u_EnableLighting", true);
            shader->setFloat("u_AmbientLighting", ambientLighting);
            if (lights.size() > 0)
            {
                for (unsigned int i = 0; i < lights.size(); i++)
                {
                    shader->setVec3("u_Light[" + std::to_string(i) + "].position", glm::vec3(lights[i]->position, 1.0f));
                    shader->setFloat("u_Light[" + std::to_string(i) + "].dist", lights[i]->radius);
                }
            }
            // filter effects
            shader->setBool("u_Outline", outline);
            shader->setFloat("u_AspectRatio", aspectRatio);
            // bind and draw
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        void SpriteRenderer::init()
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
                1.0f, 0.0f, 1.0f, 0.0f
            };

            glGenVertexArrays(1, &this->VAO);
            glGenBuffers(1, &VBO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(this->VAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

	}
}