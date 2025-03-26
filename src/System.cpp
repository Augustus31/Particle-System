#include <System.h>
#include <random>
#include <iostream>

std::random_device rd{};
std::mt19937 gen{ rd() };

extern double currTime;
extern double prevTime;

void System::InitSimpleParticleSystem() {
	creationrate = 5;
	ground = -2;
	initpos = glm::vec3(0, 2, 0);
	initposvar = glm::vec3(0.5, 0.5, 0.5);
	initvel = glm::vec3(0, 0, 0);
	initvelvar = glm::vec3(0.5, 0.5, 0.5);
	gravity = 1;
	airdensity = 2;
	friction = 0.9;
	ctime = currTime;
	particlelifetime = 5000.0;
	particlelifetimevar = 1000.0;
	particleElasticity = 0.5f;
	spawn = false;
}

void System::CreateParticle(float lifetime) {

	std::normal_distribution<float> dix(initpos.x, sqrt(initposvar.x));
	std::normal_distribution<float> diy(initpos.y, sqrt(initposvar.y));
	std::normal_distribution<float> diz(initpos.z, sqrt(initposvar.z));

	std::normal_distribution<float> dvx(initvel.x, sqrt(initvelvar.x));
	std::normal_distribution<float> dvy(initvel.y, sqrt(initvelvar.y));
	std::normal_distribution<float> dvz(initvel.z, sqrt(initvelvar.z));

	float posx = dix(gen);
	float posy = diy(gen);
	float posz = diz(gen);
	glm::vec3 pos = glm::vec3(posx, posy, posz);

	float velx = dvx(gen);
	float vely = dvy(gen);
	float velz = dvz(gen);
	glm::vec3 vel = glm::vec3(velx, vely, velz);

	Particle* p = new Particle(0.1f, pos, vel, 0.1f, particleElasticity, currTime, lifetime);
	particles.push_back(p);
}

void System::Update(float deltaTime) {
	if (currTime - ctime > (1000.0 / creationrate)) {
		std::normal_distribution<float> dl(particlelifetime, sqrt(particlelifetimevar));
		float l = dl(gen);
		if (l < 0.5) {
			l = 0.5;
		}
		CreateParticle(l);

		if (!spawn) {
			ctime = currTime;
			spawn = true;
		}
		else {
			ctime += (1000.0 / creationrate);
		}
	}

	for (int i = 0; i < particles.size(); i++) {
		if (currTime > (particles[i]->creationTime + particles[i]->lifetime)) {
			Particle* p = particles[i];
			particles.erase(particles.begin() + i);
			delete p;
			i--;
		}
	}

	deltaTime = deltaTime / 1000.0; //convert from ms to s
	float oversampleRate = 10;
	float oversample = deltaTime / (float)oversampleRate;
	for (int j = 0; j < oversampleRate; j++) {
		//Apply gravity to each particle
		for (int i = 0; i < particles.size(); i++) {
			particles[i]->ApplyForce(glm::vec3(0, -9.8 * particles[i]->GetMass() * gravity, 0));
			particles[i]->ApplyDrag(airdensity);
		}

		//Integrate to get new position
		//std::cout << deltaTime << std::endl;
		for (int i = 0; i < particles.size(); i++) {
			particles[i]->Integrate(oversample);
		}
		//std::cout << particles[0]->GetPosition().x << " " << particles[0]->GetPosition().y << " " << particles[0]->GetPosition().z << std::endl;

		for (int i = 0; i < particles.size(); i++) {
			particles[i]->GroundCheck(ground, friction);
		}

		for (int i = 0; i < particles.size(); i++) {
			particles[i]->Update();
		}
	}
}

void System::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->Draw(viewProjMtx, shader);
	}
}