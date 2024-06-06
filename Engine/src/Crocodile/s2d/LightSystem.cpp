#include "LightSystem.h"

namespace Crocodile
{
	namespace s2d
	{
		LightSystem::LightSystem()
		{

		}

		void LightSystem::addLight(Light* light)
		{
			lights.push_back(light);
		}

		void LightSystem::removeLight(Light* light)
		{
			lights.erase(std::remove(lights.begin(), lights.end(), light), lights.end());
		}

		void LightSystem::clear()
		{
			lights.clear();
		}

		std::vector<Light*> LightSystem::getScaledLights(glm::vec2 scale)
		{
			std::vector<Light*> scaledLights = {};
			for (Light* light : lights)
			{
				light->position *= scale;
				scaledLights.push_back(light);
			}
			return scaledLights;
		}
	}
}