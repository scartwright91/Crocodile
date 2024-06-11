#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../Core.h"

namespace Crocodile
{
	namespace s2d
	{
		struct CROCODILE_API Light
		{
			glm::vec2 position;
			float radius;
			glm::vec3 color;
			Light() : position(glm::vec2(0.0f)), radius(0.0f), color(glm::vec3(0.0f)) {}
		};

		class CROCODILE_API LightSystem
		{

		public:
			bool m_lightsEnabled = true;

			LightSystem();

			std::vector<Light*> inline getLights() const { return m_lights; };
			std::vector<Light*> getScaledLights(glm::vec2 scale);
			void addLight(Light* light);
			void removeLight(Light* light);
			void clear();

		private:
			std::vector<Light*> m_lights = {};

		};
	}
}


