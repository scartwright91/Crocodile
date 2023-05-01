#include "ParticleRenderer.h"


namespace Crocodile
{
	namespace s2d
	{
		ParticleRenderer::ParticleRenderer(graphics::Shader* shader)
		{
			this->shader = shader;
			init();
		}

		ParticleRenderer::~ParticleRenderer()
		{
			glDeleteVertexArrays(1, &this->VAO);
		}

		void ParticleRenderer::render(
			std::vector<Particle> particles,
			glm::mat4 model,
			glm::mat4 view,
			glm::mat4 projection,
			glm::vec3 spriteColor
		)
		{
			// use additive blending to give it a 'glow' effect
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			shader->use();
			shader->setMat4("u_Model", model);
			shader->setMat4("u_View", view);
			shader->setMat4("u_Projection", projection);
			shader->setVec3("u_Color", spriteColor);

			for (unsigned int i = 0; i < particles.size(); ++i)
			{
				Particle& p = particles[i];
				if (p.life > 0.0f)
				{
					shader->setVec2("u_Offset", p.position);
					shader->setFloat("u_Alpha", p.life);
					shader->setFloat("u_Scale", p.scale);

					// draw buffer
					glBindVertexArray(VAO);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glBindVertexArray(0);
				}
			}
			// reset blending
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void ParticleRenderer::init()
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