#pragma once

#include <algorithm>
#include <random>
#include <glm/glm.hpp>

#include "../Core.h"
#include "Object.h"

namespace Crocodile
{
	namespace s2d
	{

		struct CROCODILE_API Particle
		{
			glm::vec2 position, velocity;
			float life;
			float scale;
			bool alive;
			Particle() : position(0.0f), velocity(0.0f), life(1.0f), scale(10.0f), alive(true) {}
		};

		class CROCODILE_API ParticleGenerator : public Object
		{

		public:

			bool active = true;
			bool screenParticles = false;
			std::vector<Particle> particles;
			unsigned int width, height;

			ParticleGenerator(
				float x,
				float y,
				float scale,
				unsigned int amount,
				float duration
			);
			ParticleGenerator(
				unsigned int width,
				unsigned int height,
				float scale,
				unsigned int amount
			);
			~ParticleGenerator();

			void update(float dt);
			
			unsigned int getNParticles();

		private:

			bool local;
			unsigned int amount = 0;
			float duration = 1.0f;
			float scale = 1.0f;

			Particle createParticle();
			void init();

		};
	}
}
