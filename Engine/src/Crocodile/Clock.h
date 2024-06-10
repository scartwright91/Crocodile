#pragma once

#include <cmath>
#include <GLFW/glfw3.h>
#include "Core.h"

namespace Crocodile
{
	class CROCODILE_API Clock
	{

	public:

		Clock() {}

		void tick()
		{
			float currentFrame = (float)glfwGetTime();
			if (firstFrame)
			{
				deltaTime = 0.f;
				firstFrame = false;
				return;
			}
			deltaTime = currentFrame - lastFrame;
			if (deltaTime > 0.1)
				deltaTime = 0.0;
			lastFrame = currentFrame;
		}

		int getFPS() const
		{
			return (int)floor(1 / deltaTime);
		}

	private:
		float lastFrame = 0.0f;
		bool firstFrame = true;

	public:
		float deltaTime = 0.0f;

	};
}
