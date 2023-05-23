#include "Animation.h"

namespace Crocodile
{
	namespace s2d
	{
		Animation::Animation(ResourceManager::TextureData spritesheet, float width, unsigned int gridSize, float frameDuration)
		{
			this->spritesheetAnimation = true;
			this->spritesheet = spritesheet;
			this->width = width;
			this->gridSize = gridSize;
			this->frameDuration = frameDuration;
			this->totalFrames = (unsigned int)width / gridSize;
			this->currentFrame = 0;
		}

		Animation::Animation(std::vector<ResourceManager::TextureData> textures, float frameDuration)
		{
			this->spritesheetAnimation = false;
			this->textures = textures;
			this->frameDuration = frameDuration;
			this->totalFrames = textures.size();
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
			timer += dt;
			if (timer > frameDuration)
			{
				currentFrame = (currentFrame + 1) % totalFrames;
				timer = 0.0f;
			}
		}
	}
}
