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
			// Constructor for animations contained within a spritesheet
			Animation(TextureData spritesheet, float width, unsigned int gridSize, float frameDuration);
			// Constructor for animations stored as separate images
			Animation(std::vector<TextureData> textures, float frameDuration);
			~Animation();

			bool spritesheetAnimation = false;

			// animation variables
			bool freezeAnimation = false;
			bool repeat = true;
			bool finished = false;
			float delayBetweenAnimations = 0.f;

			// spritesheet variables
			TextureData spritesheet = {};
			unsigned int gridSize;

			float frameDuration;
			float width;
			std::vector<TextureData> textures = {};
			unsigned int totalFrames, currentFrame;

			glm::vec2 getTextureOffset();
			void updateAnimation(float dt);

		private:
			float timer = 0.0f;
			float finishTimer = 0.0f;
		};
	}
}
