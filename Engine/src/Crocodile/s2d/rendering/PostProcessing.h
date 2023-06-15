#pragma once

#include <glm/glm.hpp>
#include "../../Core.h"
#include "../../graphics/Shader.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API PostProcessing
		{

		public:
			enum PostProcessingEffect
			{
				None,
				GREYSCALE,
				WAVEY
			};

			enum TransitionEffect
			{
				NO_TRANSITION,
				FADE,
				DIAMOND
			};

			unsigned int textureColorbuffer;
			unsigned int width, height;

			PostProcessing(graphics::Shader *shader, unsigned int width, unsigned int height);
			~PostProcessing();

			void beginRender();
			void endRender();
			void render(
				float dt,
				bool greyscale,
				bool wavey,
				bool screenShake,
				TransitionEffect effect,
				bool fadeinTransition,
				bool fadeoutTransition,
				float transitionCounter,
				float width,
				float height);

			void createTexture(unsigned int width, unsigned int height);

		private:
			graphics::Shader *shader = nullptr;
			unsigned int framebuffer, RBO;
			unsigned int VAO;

			void init();
		};
	}
}
