#pragma once

#include <vector>
#include <Particle.h>

class System {
public:
	void InitSimpleParticleSystem();
	void CreateParticle(float lifetime);
	void Update(float deltaTime);
	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	float creationrate;
	float particlelifetime;
	float particlelifetimevar;
	float particleElasticity;
	float ground;
	float gravity;
	float airdensity;
	float friction;
	glm::vec3 initpos;
	glm::vec3 initposvar;
	glm::vec3 initvel;
	glm::vec3 initvelvar;

private:
	float ctime;
	bool spawn;
	std::vector<Particle*> particles;
};