#pragma once
#include "core.h"
#include <vector>

class Particle {
public:
	void ApplyForce(glm::vec3& f);
	void Integrate(float deltaTime);
	void Update();
	void Draw(const glm::mat4& viewProjMtx, GLuint shader);
	glm::vec3 GetPosition();
	glm::vec3 GetVelocity();
	float GetMass();
	void GroundCheck(float ground, float friction);
	void move(glm::vec3 dir);
	void ApplyDrag(float airdensity);
	Particle(float mass, glm::vec3 position, glm::vec3 vel, float radius, float elasticity, double creationTime, double lifetime);
	void SetMesh(std::vector<glm::vec3>* positions, std::vector<glm::vec3>* normals, std::vector<unsigned int>* indices, int layersH, int layersW);
	bool fixed;
	double creationTime;
	double lifetime;
private:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;
	float radius;
	float mass;
	float elasticity;
	int index;

	//rendering
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	glm::mat4 model;
	glm::vec3 color;

	// Cube Information
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices; 
};