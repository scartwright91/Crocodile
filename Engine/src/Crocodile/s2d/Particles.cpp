#include "Particles.h"

namespace Crocodile
{
	namespace s2d
	{

		ParticleGenerator::ParticleGenerator(ParticleSettings settings) : settings(settings)
		{
			this->size = glm::vec2(1.f);
			this->color = settings.colour;
			if (settings.texture.name != "")
				setTexture(settings.texture);
			renderMethod = "particles";
			createParticles();
		}

		ParticleGenerator::~ParticleGenerator()
		{
			particles.clear();
		}

		void ParticleGenerator::update(float dt)
		{
			// update particle duration
			if (settings.duration > 0.f)
			{
				settings.duration -= dt;
				if (settings.duration < 0 && active)
					active = false;
			}

			if ((particles.size() == 0) && (settings.createOnce))
				finished = true;

			// update all particles
			for (unsigned int i = 0; i < getNParticles(); ++i)
			{
				Particle &p = particles[i];
				if (p.alive)
					p.life -= dt; // reduce life
				if (p.life > 0.0f)
				{ // particle is alive, thus update
					glm::vec2 velocity = {0.0, 0.0};
					velocity.x = glm::cos(p.direction) * p.speed * dt;
					velocity.y = glm::sin(p.direction) * p.speed * dt;
					// if (settings.applyGravity)
					// 	velocity.y += 200.f * dt;
					p.position += velocity;
				}
				else if (active && !settings.createOnce)
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
			if (settings.w > 0)
			{
				float r = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
				p.position.x += (r * settings.w);
			}
			if (settings.h > 0)
			{
				float r = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
				p.position.y += (r * settings.h);
			}
			p.scale = settings.scale;
			p.life = settings.life + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.5));

			float disp = -settings.dispersion + 2 * settings.dispersion * static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
			p.speed = settings.speed;
			if (settings.type == SCATTER)
			{
				float r = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
				p.speed *= r;
			}

			p.direction = settings.direction + disp;

			return p;
		}

		void ParticleGenerator::createParticles()
		{
			particles = {};
			for (unsigned int i = 0; i < settings.amount; ++i)
				particles.push_back(createParticle());
		}

	}
}