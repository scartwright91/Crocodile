#pragma once

#include <vector>
#include "Renderer.h"
#include "../Particles.h"
#include "../LightSystem.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API ParticleRenderer : public Renderer
		{

		public:
			ParticleRenderer(graphics::Shader *shader);

			void render(
				std::vector<Particle> particles,
				glm::mat4 model,
				glm::mat4 view,
				glm::mat4 projection,
				unsigned int textureID,
				bool useTexture,
				glm::vec3 spriteColor,
				float alpha,
				float layerAlpha,
                float ambientLighting,
                std::vector<Light *> lights
				);

		private:
			void initShader();
		};
	}
}
