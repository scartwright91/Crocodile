#include "Animation.h"

namespace Crocodile
{
	namespace s2d
	{
		Animation::Animation(TextureData spritesheet, float width, unsigned int gridSize, float frameDuration)
		{
			m_spritesheetAnimation = true;
			m_spritesheet = spritesheet;
			m_width = width;
			m_gridSize = gridSize;
			m_frameDuration = frameDuration;
			m_totalFrames = (unsigned int)width / gridSize;
			m_currentFrame = 0;
		}

		Animation::Animation(std::vector<TextureData> textures, float frameDuration) : m_gridSize(0), m_width(0.0f)
		{
			m_spritesheetAnimation = false;
			m_textures = textures;
			m_frameDuration = frameDuration;
			m_totalFrames = (int)textures.size();
			m_currentFrame = 0;
		}

		Animation::~Animation()
		{
		}

		glm::vec2 Animation::getTextureOffset()
		{
			return glm::vec2((float)m_gridSize * m_currentFrame / m_width, 0.0f);
		}

		void Animation::updateAnimation(float dt)
		{
			if (!m_freezeAnimation)
			{
				if (!m_finished)
				{
					m_timer += dt;
					if (m_timer > m_frameDuration)
					{
						m_currentFrame += 1;
						m_timer = 0.0f;
						if (m_currentFrame == m_totalFrames)
						{
							m_finishTimer = (float)glfwGetTime();
							m_finished = true;
							m_currentFrame = 0;
						}
					}
				}
				else
				{
					if (!m_repeat)
						return;
					if (glfwGetTime() - m_finishTimer > m_delayBetweenAnimations)
					{
						m_finished = false;
						m_timer = 0.0f;
					}
				}
			}
		}
	}
}
