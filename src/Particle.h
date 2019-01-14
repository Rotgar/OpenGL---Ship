#ifndef PARTICLES_H
#define PARTICLES_H
#include <vector>
#include <random>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Particle
{
	glm::vec3 Position;
	glm::vec4 Color;
	GLfloat Life;
	unsigned int nr_particles, lastUsedParticle;
	

public:
	Particle()
		: Position(0.0f), Color(1.0f), Life(0.0f), nr_particles(1000), lastUsedParticle(0) { 

	}
	std::vector<Particle> particles;
	void initParticles();
	void RespawnParticle(Particle &particle);
	int FirstUnusedParticle();
	void updateParticles(float dt, float sM, bool stand);

	GLfloat getLife() {
		return Life;
	}
	glm::vec3 getPosition()
	{
		return Position;
	}
	glm::vec4 getColor()
	{
		return Color;
	}
};



#endif // !PARTICLES_H

