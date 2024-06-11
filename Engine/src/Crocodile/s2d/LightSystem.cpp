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
			m_lights.push_back(light);
		}

		void LightSystem::removeLight(Light* light)
		{
			m_lights.erase(std::remove(m_lights.begin(), m_lights.end(), light), m_lights.end());
		}

		void LightSystem::clear()
		{
			m_lights.clear();
		}

		std::vector<Light*> LightSystem::getScaledLights(glm::vec2 scale)
		{
			std::vector<Light*> scaledLights = {};
			for (Light* light : m_lights)
			{
				light->position *= scale;
				scaledLights.push_back(light);
			}
			return scaledLights;
		}
	}
}