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

		enum CROCODILE_API ParticleType
		{
			SCATTER,
			RADIAL
		};

		struct CROCODILE_API Particle
		{
			glm::vec2 position;
			float direction;
			float speed;
			float life;
			float scale;
			bool alive;
			Particle() : position(0.0f), direction(0.0f), life(1.0f), scale(10.0f), alive(true) {}
		};

		struct CROCODILE_API ParticleSettings
		{
			ParticleType type = SCATTER;
			glm::vec3 colour = glm::vec3(1.f);
			ResourceManager::TextureData texture;
			float w = 0;
			float h = 0;
			float direction = 0.5f;
			float dispersion = 3.14f;
			float scale = 1.0f;
			float speed = 300.0f;
			float life = .5f;
			unsigned int amount = 50;
			float duration = 0.0f;
			bool createOnce = false;
			bool applyGravity = false;
		};

		class CROCODILE_API ParticleGenerator : public Object
		{

		public:
			bool active = true;
			std::vector<Particle> particles;

			ParticleGenerator(ParticleSettings settings);
			~ParticleGenerator();

			void update(float dt);

			unsigned int getNParticles();

			ParticleSettings settings;

			bool finished = false;

			void createParticles();

		private:
			Particle createParticle();
		};
	}
}
