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
			bool lightsEnabled = true;
			std::vector<Light*> lights = {};

			LightSystem();
			~LightSystem();

			void addLight(Light* light);
			void removeLight(Light* light);
			void clear();

			std::vector<Light*> getScaledLights(glm::vec2 scale);

		};
	}
}


