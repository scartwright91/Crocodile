#include "LightSystem.h"

namespace Crocodile
{
	namespace s2d
	{
		LightSystem::LightSystem()
		{

		}

		LightSystem::~LightSystem()
		{

		}

		void LightSystem::addLight(Light* light)
		{
			lights.push_back(light);
		}

		void LightSystem::removeLight(Light* light)
		{

		}

		void LightSystem::clear()
		{
			lights = {};
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