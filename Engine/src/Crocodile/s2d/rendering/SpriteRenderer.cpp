#include "SpriteRenderer.h"

namespace Crocodile
{
    namespace s2d
    {
        SpriteRenderer::SpriteRenderer(graphics::Shader *shader) : Renderer(shader)
        {
            initShader();
        }

        void SpriteRenderer::render(
            float time,
            glm::mat4 model,
            glm::mat4 view,
            glm::mat4 projection,
            TextureData texture,
            bool useTexture,
            bool useColorTexture,
            float numberOfRows,
            float numberOfCols,
            glm::vec2 textureOffset,
            glm::vec3 spriteColor,
            float alpha,
            float ambientLighting,
            std::vector<Light *> lights,
            bool outline,
            float aspectRatio,
            unsigned int distortionTexture,
            bool useDistortionTexture,
            bool scrollDistortionX,
            bool scrollDistortionY,
            float distortionSpeed,
            bool flipX,
            bool flipY,
            float layerAlpha,
            bool useSqueeze,
            glm::vec2 deformation
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
            shader->setBool("u_UseColorTexture", useColorTexture);
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
                glBindTexture(GL_TEXTURE_2D, texture.textureID);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, distortionTexture);
                shader->setFloat("u_NumberOfRows", numberOfRows);
                shader->setFloat("u_NumberOfCols", numberOfCols);
                shader->setVec2("u_TextureOffset", textureOffset);
                shader->setFloat("u_TextureWidth", texture.width);
                shader->setFloat("u_TextureHeight", texture.height);
            }
            shader->setVec3("u_SpriteColor", spriteColor);
            shader->setFloat("u_Alpha", alpha);
            shader->setFloat("u_LayerAlpha", layerAlpha);
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
            // squeeze effect
            shader->setBool("u_Squeeze", useSqueeze);
            shader->setVec2("u_Deformation", deformation);
            // bind and draw
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        void SpriteRenderer::initShader()
        {
            this->shader->use();
            this->shader->setInt("u_Texture", 0);
            this->shader->setInt("u_DistortionTexture", 1);
        }

    }
}