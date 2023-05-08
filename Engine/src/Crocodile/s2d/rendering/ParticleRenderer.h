#pragma once

#include <vector>
#include "../../Core.h"
#include "../../graphics/Shader.h"
#include "../Particles.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API ParticleRenderer
		{

		public:
			ParticleRenderer(graphics::Shader *shader);
			~ParticleRenderer();

			void render(
				std::vector<Particle> particles,
				glm::mat4 model,
				glm::mat4 view,
				glm::mat4 projection,
				unsigned int textureID,
				bool useTexture,
				glm::vec3 spriteColor,
				float alpha);

		private:
			graphics::Shader *shader;
			unsigned int VAO = 0;

			void init();
		};
	}
}
