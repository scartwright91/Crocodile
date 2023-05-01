#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "../Core.h"

namespace Crocodile
{
	namespace s2d
	{
		class CROCODILE_API Animation
		{
		public:

			bool spritesheetAnimation = false;
			unsigned int spritesheetID;
			unsigned int gridSize;
			float frameDuration;
			float width;
			std::vector<unsigned int> textures = {};
			unsigned int totalFrames, currentFrame;

			Animation(unsigned int spritesheetID, float width, unsigned int gridSize, float frameDuration);
			Animation(std::vector<unsigned int> textures, float frameDuration);
			~Animation();

			glm::vec2 getTextureOffset();
			void updateAnimation(float dt);

		private:

			float timer = 0.0f;
		};
	}
}

