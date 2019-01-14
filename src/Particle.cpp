#include "Particle.h"
#include "Defines.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

void Particle::initParticles()
{
	for (unsigned int i = 0; i < nr_particles; ++i)
	{
		particles.push_back(Particle());
	}
}
int Particle::FirstUnusedParticle()
{
	for (GLuint i = lastUsedParticle; i < nr_particles; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Override first particle if all others are alive
	lastUsedParticle = 0;
	return 0;
}
void Particle::RespawnParticle(Particle &particle)
{
	particle.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	particle.Color = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
	particle.Life = 1.5f;
}
void Particle::updateParticles(float dt, float sM, bool stand)
{
	unsigned int new_partices = 0;
	if (!stand)
		new_partices = 2;
	for (unsigned int i = 0; i < new_partices; ++i)
	{
		int unusedParticle = FirstUnusedParticle();
		RespawnParticle(particles[unusedParticle]);
	}
	for (GLuint i = 0; i < nr_particles; ++i)
	{
		Particle &p = particles[i];
		p.Life -= 0.4f * dt; // reduce life
		if (p.Life > 0.0f)
		{	// particle is alive, thus update
			std::random_device rd; // obtain a random number from hardware
			std::mt19937 eng(rd()); // seed the generator
			std::uniform_int_distribution<> distrX(0.0f, 20.0f); // define the range
			std::uniform_int_distribution<> distrZ(-15.0f, 15.0f); // define the range
			float x = (float)distrX(eng);
			float y = (float)distrX(eng);
			float z = (float)distrZ(eng);
			p.Position.y += y * dt;
			if (sM < 0.0f)
			{
				p.Position.x += x * dt - sM * 0.6f;// +sM * 10;
			}
			else
			{
				p.Position.x -= x * dt + sM * 0.6f;// -sM * 10;
			}
			p.Position.z += z * dt * 1.0f;
			p.Color.x += 0.003f;
			p.Color.y += 0.003f;
			p.Color.z += 0.003f;
		}
	}
	std::sort(particles.begin(), particles.end(), [](const auto& lhs, const auto& rhs)
	{
		return lhs.Position.x < rhs.Position.x;
	});
}