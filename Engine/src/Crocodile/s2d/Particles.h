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
			std::vector<Particle> particles;

			ParticleGenerator(unsigned int amount);
			~ParticleGenerator();

			void update(float dt);

			unsigned int getNParticles();

			// Particle attributes
			float w = 0;
			float h = 0;
			float direction = 0.5;
			float dispersion = 0.1;
			float scale = 1.0f;
			float velocity = 1.0f;

		private:
			unsigned int amount = 0;
			float duration = 1.0f;
			Particle createParticle();
			void init();
		};
	}
}
