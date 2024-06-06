#include "Animation.h"

namespace Crocodile
{
	namespace s2d
	{
		Animation::Animation(TextureData spritesheet, float width, unsigned int gridSize, float frameDuration)
		{
			this->spritesheetAnimation = true;
			this->spritesheet = spritesheet;
			this->width = width;
			this->gridSize = gridSize;
			this->frameDuration = frameDuration;
			this->totalFrames = (unsigned int)width / gridSize;
			this->currentFrame = 0;
		}

		Animation::Animation(std::vector<TextureData> textures, float frameDuration) : gridSize(0), width(0.0f)
		{
			this->spritesheetAnimation = false;
			this->textures = textures;
			this->frameDuration = frameDuration;
			this->totalFrames = (int)textures.size();
			this->currentFrame = 0;
		}

		Animation::~Animation()
		{
		}

		glm::vec2 Animation::getTextureOffset()
		{
			return glm::vec2((float)gridSize * currentFrame / width, 0.0f);
		}

		void Animation::updateAnimation(float dt)
		{
			if (!freezeAnimation)
			{
				if (!finished)
				{
					timer += dt;
					if (timer > frameDuration)
					{
						currentFrame += 1;
						timer = 0.0f;
						if (currentFrame == totalFrames)
						{
							finishTimer = (float)glfwGetTime();
							finished = true;
							currentFrame = 0;
						}
					}
				}
				else
				{
					if (!repeat)
						return;
					if (glfwGetTime() - finishTimer > delayBetweenAnimations)
					{
						finished = false;
						timer = 0.0f;
					}
				}
			}
		}
	}
}
