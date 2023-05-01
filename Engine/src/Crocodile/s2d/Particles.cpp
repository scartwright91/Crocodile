#include "Particles.h"

namespace Crocodile
{
	namespace s2d
	{

		ParticleGenerator::ParticleGenerator(
			float x,
			float y,
			float scale,
			unsigned int amount,
			float duration) : local(true)
		{
			move(x, y);
			this->scale = scale;
			this->size = glm::vec2(1.0f);
			this->amount = amount;
			this->duration = duration;

			renderType = PARTICLE;
			init();
		}

		ParticleGenerator::ParticleGenerator(
			unsigned int width,
			unsigned int height,
			float scale,
			unsigned int amount) : local(false), width(width), height(height)
		{
			this->scale = scale;
			this->size = glm::vec2(1.0f);
			this->amount = amount;
			this->duration = 0.f;
			this->screenParticles = true;

			renderType = PARTICLE;
			init();
		}

		ParticleGenerator::~ParticleGenerator()
		{
		}

		void ParticleGenerator::update(float dt)
		{
			// update particle duration
			if (duration > 0.f)
			{
				duration -= dt;
				if (duration < 0 && active)
					active = false;
			}

			// update all particles
			for (unsigned int i = 0; i < getNParticles(); ++i)
			{
				Particle &p = particles[i];
				if (p.alive)
					p.life -= dt; // reduce life
				if (p.life > 0.0f)
				{ // particle is alive, thus update
					p.position -= p.velocity * dt;
				}
				else if (active)
				{
					// reset particle attributes
					p = createParticle();
				}
				else
				{
					// remove particle
					particles.erase(particles.begin() + i);
				}
			}
		}

		unsigned int ParticleGenerator::getNParticles()
		{
			return (unsigned int)particles.size();
		}

		Particle ParticleGenerator::createParticle()
		{
			std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_int_distribution<int> randomWidth(0, width);
			std::uniform_int_distribution<int> randomHeight(0, height);

			Particle p;
			if (local)
			{
				p.position = glm::vec2(0.0f);
				p.scale = scale;
			}
			else
			{
				p.position = glm::vec2(randomWidth(rng), randomHeight(rng));
				p.scale = scale * randomWidth(rng) / width;
			}
			// random float between -1 and 1
			float velX = -1.f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2));
			float velY = -1.f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2));
			float life = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.5));
			p.velocity = glm::vec2(100.0f * velX, 100.0f * velY);
			p.life = life;
			return p;
		}

		void ParticleGenerator::init()
		{
			for (unsigned int i = 0; i < this->amount; ++i)
				particles.push_back(createParticle());
		}

	}
}