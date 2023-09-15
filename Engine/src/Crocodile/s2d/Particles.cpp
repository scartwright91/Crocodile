#include "Particles.h"

namespace Crocodile
{
	namespace s2d
	{

		ParticleGenerator::ParticleGenerator(unsigned int amount)
		{
			this->amount = amount;
			this->size = glm::vec2(1.0f);
			renderMethod = "particles";
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

			if (particles.size() == 0)
				finished = true;

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
				else if (active && !createOnce)
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

			Particle p;

			// assign particle attributes
			p.position = glm::vec2(0.0f);
			if (w > 0)
			{
				float r = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
				p.position.x += (r * w);
			}
			if (h > 0)
			{
				float r = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
				p.position.y += (r * h);
			}
			p.scale = scale;
			p.life = life + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.5));

			// calculate velocity
			float dispX = -dispersion + 2 * dispersion * static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
			float dispY = -dispersion + 2 * dispersion * static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));

			p.velocity.x = glm::cos(direction + dispX) * velocity * 100.f;
			p.velocity.y = glm::sin(direction + dispY) * velocity * 100.f;
			return p;
		}

		void ParticleGenerator::createParticles()
		{
			for (unsigned int i = 0; i < this->amount; ++i)
				particles.push_back(createParticle());
		}

	}
}