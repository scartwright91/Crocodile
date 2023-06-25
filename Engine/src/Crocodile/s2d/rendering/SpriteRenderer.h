#pragma once

#include <glm/glm.hpp>

#include "Renderer.h"
#include "../LightSystem.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API SpriteRenderer : public Renderer
		{

		public:
			SpriteRenderer(graphics::Shader *shader);

			void render(
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
				float layerAlpha);

		private:
			void initShader();
		};
	}
}
