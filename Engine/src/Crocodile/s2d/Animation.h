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

			bool m_spritesheetAnimation = false;

			// animation variables
			bool m_freezeAnimation = false;
			bool m_repeat = true;
			bool m_finished = false;
			float m_delayBetweenAnimations = 0.f;

			// spritesheet variables
			TextureData m_spritesheet = {};
			unsigned int m_gridSize;

			float m_frameDuration;
			float m_width;
			std::vector<TextureData> m_textures = {};
			unsigned int m_totalFrames, m_currentFrame;

			glm::vec2 getTextureOffset();
			void updateAnimation(float dt);

		private:
			float m_timer = 0.0f;
			float m_finishTimer = 0.0f;
		};
	}
}
