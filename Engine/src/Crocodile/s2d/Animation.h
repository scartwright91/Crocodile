#pragma once

#include "Crocodile/ResourceManager.h"
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
			ResourceManager::TextureData spritesheet = {};
			unsigned int gridSize;
			float frameDuration;
			float width;
			std::vector<ResourceManager::TextureData> textures = {};
			unsigned int totalFrames, currentFrame;

			Animation(ResourceManager::TextureData spritesheet, float width, unsigned int gridSize, float frameDuration);
			Animation(std::vector<ResourceManager::TextureData> textures, float frameDuration);
			~Animation();

			glm::vec2 getTextureOffset();
			void updateAnimation(float dt);

		private:
			float timer = 0.0f;
		};
	}
}
