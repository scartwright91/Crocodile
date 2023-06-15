#pragma once

#include "../../Core.h"

#include "../../graphics/Shader.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API BackgroundGrid
		{

		public:
			BackgroundGrid(graphics::Shader *shader);
			~BackgroundGrid();

			void render(
				float width,
				float height,
				float offsetX,
				float offsetY,
				float zoom);

			bool active = false;
			unsigned int gridSize = 0;
			float alpha = 1.0f;
			glm::vec3 color1 = glm::vec3(1.f);
			glm::vec3 color2 = glm::vec3(.5f);

		private:
			graphics::Shader *shader;
			unsigned int VAO = 0;

			void init();
		};
	}
}
