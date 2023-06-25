#include "ParticleRenderer.h"

namespace Crocodile
{
	namespace s2d
	{
		ParticleRenderer::ParticleRenderer(graphics::Shader *shader) : Renderer(shader)
		{
			initShader();
		}

		void ParticleRenderer::render(
			std::vector<Particle> particles,
			glm::mat4 model,
			glm::mat4 view,
			glm::mat4 projection,
			unsigned int textureID,
			bool useTexture,
			glm::vec3 spriteColor,
			float alpha,
			float layerAlpha)
		{
			// use additive blending to give it a 'glow' effect
			// glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			shader->use();
			shader->setMat4("u_Model", model);
			shader->setMat4("u_View", view);
			shader->setMat4("u_Projection", projection);
			shader->setBool("u_UseTexture", useTexture);

			if (useTexture)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureID);
			}
			else
			{
				shader->setVec3("u_Color", spriteColor);
			}
			shader->setFloat("u_LayerAlpha", layerAlpha);

			for (unsigned int i = 0; i < particles.size(); ++i)
			{
				Particle &p = particles[i];
				if (p.life > 0.0f)
				{
					shader->setVec2("u_Offset", p.position);
					shader->setFloat("u_Alpha", alpha * p.life);
					shader->setFloat("u_Scale", p.scale);

					// draw buffer
					glBindVertexArray(VAO);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glBindVertexArray(0);
				}
			}
			// reset blending
			// aaglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void ParticleRenderer::initShader()
		{
			this->shader->use();
			this->shader->setInt("u_Texture", 0);
		}
	}
}