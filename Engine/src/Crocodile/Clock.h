#pragma once

#include <cmath>
#include <GLFW/glfw3.h>
#include "Core.h"

namespace Crocodile
{
	class CROCODILE_API Clock
	{

	public:
		float deltaTime = 0.0f;

		Clock()
		{
		}
		~Clock()
		{
		}

		void tick()
		{
			float currentFrame = (float)glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
		}

		int getFPS()
		{
			return (int)floor(1 / deltaTime);
		}

	private:
		float lastFrame = 0.0f;
	};
}
