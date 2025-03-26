#include <Particle.h>
#include <numbers>
#include <iostream>

constexpr double pi = 3.14159265358979323846;

Particle::Particle(float mass, glm::vec3 position, glm::vec3 vel, float radius, float elasticity, double creationTime, double lifetime) {
	this->mass = mass;
	this->position = position;
	this->velocity = vel;
	this->radius = radius;
	this->creationTime = creationTime;
	this->lifetime = lifetime;
	this->force = glm::vec3(0);
	this->fixed = false;
	this->elasticity = elasticity;

	//set up rendering, currently cube but will be changed to sphere
    
    glm::vec3 cubeMin(-0.5, -0.5, -0.5);
    glm::vec3 cubeMax(0.5, 0.5, 0.5);

    SetMesh(&positions, &normals, &indices, 7, 8);

    //positions = {
    //    // Front
    //    glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
    //    glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
    //    glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
    //    glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),

    //    // Back
    //    glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
    //    glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
    //    glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),
    //    glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),

    //    // Top
    //    glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
    //    glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
    //    glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
    //    glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

    //    // Bottom
    //    glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
    //    glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
    //    glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
    //    glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),

    //    // Left
    //    glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
    //    glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
    //    glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
    //    glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

    //    // Right
    //    glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
    //    glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
    //    glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
    //    glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z) };

    //// Specify normals
    //normals = {
    //    // Front
    //    glm::vec3(0, 0, 1),
    //    glm::vec3(0, 0, 1),
    //    glm::vec3(0, 0, 1),
    //    glm::vec3(0, 0, 1),

    //    // Back
    //    glm::vec3(0, 0, -1),
    //    glm::vec3(0, 0, -1),
    //    glm::vec3(0, 0, -1),
    //    glm::vec3(0, 0, -1),

    //    // Top
    //    glm::vec3(0, 1, 0),
    //    glm::vec3(0, 1, 0),
    //    glm::vec3(0, 1, 0),
    //    glm::vec3(0, 1, 0),

    //    // Bottom
    //    glm::vec3(0, -1, 0),
    //    glm::vec3(0, -1, 0),
    //    glm::vec3(0, -1, 0),
    //    glm::vec3(0, -1, 0),

    //    // Left
    //    glm::vec3(-1, 0, 0),
    //    glm::vec3(-1, 0, 0),
    //    glm::vec3(-1, 0, 0),
    //    glm::vec3(-1, 0, 0),

    //    // Right
    //    glm::vec3(1, 0, 0),
    //    glm::vec3(1, 0, 0),
    //    glm::vec3(1, 0, 0),
    //    glm::vec3(1, 0, 0) };

    //// Specify indices
    //indices = {
    //    0, 1, 2, 0, 2, 3,        // Front
    //    4, 5, 6, 4, 6, 7,        // Back
    //    8, 9, 10, 8, 10, 11,     // Top
    //    12, 13, 14, 12, 14, 15,  // Bottom
    //    16, 17, 18, 16, 18, 19,  // Left
    //    20, 21, 22, 20, 22, 23,  // Right
    //};

    // Model matrix.
    model = glm::mat4(0.5f);
    model[0][0] = radius;
    model[1][1] = radius;
    model[2][2] = radius;

    model[3][0] = position.x;
    model[3][1] = position.y;
    model[3][2] = position.z;


    // The color of the cube. Try setting it to something else!
    color = glm::vec3(0.7, 0.3, 0.3);

    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
    glGenBuffers(1, &EBO);

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind the EBO to the bound VAO and send the data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Particle::Update() {
    model[3][0] = position.x;
    model[3][1] = position.y;
    model[3][2] = position.z;

    //std::cout << position.x << " " << position.y << " " << position.z << " " << std::endl;
}

void Particle::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
    // actiavte the shader program
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    // Bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void Particle::ApplyForce(glm::vec3& f) {
	force += f;
}

void Particle::Integrate(float deltaTime) {
	if (!fixed) {
		glm::vec3 accel = force / mass; // Apply Newton’s Second Law (f=ma)
		velocity += accel * deltaTime; // Forward Euler integration to get new velocity
		position += velocity * deltaTime; // Backward Euler integration to get new position
		force = glm::vec3(0); // Zero force out so next frame will start fresh
	}
	else {
		force = glm::vec3(0); // Zero force out so next frame will start fresh
	}

}

glm::vec3 Particle::GetPosition() {
	return position;
}

glm::vec3 Particle::GetVelocity() {
	return velocity;
}

float Particle::GetMass() {
	return mass;
}

void Particle::GroundCheck(float ground, float friction) { //elasticity and friction between 0.0 and 1.0
	if (position.y < ground) {
		position.y = ground + (ground - position.y);
		glm::vec3 groundNormal = glm::vec3(0.0, 1.0, 0.0);
		velocity = velocity - (1.0f + elasticity) * glm::dot(velocity, groundNormal) * groundNormal;

		//friction simulation
		velocity.x = velocity.x * friction;
		velocity.z = velocity.z * friction;

		if (glm::length(velocity) < 0.01) {
			velocity = glm::vec3(0);
		}
	}
}

void Particle::move(glm::vec3 dir) {
	position += dir;
}

void Particle::ApplyDrag(float airdensity) {
	glm::vec3 vel = velocity;
	if (length(vel) > 0.001) {
		float a0 = pi * radius * radius;
		float a = a0;
		glm::vec3 f = -0.5f * airdensity * 1.225f * 1.28f * glm::length(vel) * glm::length(vel) * a * glm::normalize(vel);
		ApplyForce(f);

		//Debug
		//std::cout << "particle velocity" << std::endl;
		//std::cout << vel.x << " " << vel.y << " " << vel.z << std::endl;

		//std::cout << "cross sectional area" << std::endl;
		//std::cout << a << std::endl;

		//std::cout << "force" << std::endl;
		//std::cout << f.x << " " << f.y << " " << f.z << std::endl;

		//std::cout << "---------------------------------" << std::endl;
	}
}

void Particle::SetMesh(std::vector<glm::vec3>* positions, std::vector<glm::vec3>* normals, std::vector<unsigned int>* indices, int layersH, int layersW) {
    positions->push_back(glm::vec3(0, 1, 0));
    float y = 1.0;
    for (int i = 0; i < layersH; i++) {
        y = cos(pi * (i + 1) / (layersH + 1));
        float hypotenuse = sqrtf(1.0 - y * y);
        float theta = 0;
        for (int j = 0; j < layersW; j++) {
            float x = hypotenuse * cosf(theta);
            float z = hypotenuse * sinf(theta);
            positions->push_back(glm::vec3(x, y, z));
            theta += ((2.0 * pi) / (layersW));
        }
    }
    positions->push_back(glm::vec3(0, -1, 0));

    for (int i = 0; i < layersH; i++) {
        for (int j = 0; j < layersW; j++) {
            if (i == 0) {
                if (j == layersW - 1) {
                    indices->push_back(j + 1);
                    indices->push_back(1);
                    indices->push_back(0);
                }
                else {
                    indices->push_back(j + 1);
                    indices->push_back(j + 2);
                    indices->push_back(0);
                }
            }
            else {
                if (j == layersW - 1) {
                    indices->push_back(i * layersW + j + 1);
                    indices->push_back(i * layersW + 1);
                    indices->push_back((i - 1) * layersW + 1);

                    indices->push_back(i * layersW + j + 1);
                    indices->push_back((i - 1) * layersW + 1);
                    indices->push_back((i - 1) * layersW + j + 1);
                }
                else {
                    indices->push_back(i * layersW + j + 1);
                    indices->push_back(i * layersW + j + 2);
                    indices->push_back((i - 1) * layersW + j + 2);

                    indices->push_back(i * layersW + j + 1);
                    indices->push_back((i - 1) * layersW + j + 2);
                    indices->push_back((i - 1) * layersW + j + 1);
                }
            }
        }
    }

    int last = layersH * layersW + 1;
    int lroffset = layersW * (layersH - 1);

    for (int j = 0; j < layersW; j++) {
        if (j == layersW - 1) {
            indices->push_back(lroffset + j + 1);
            indices->push_back(lroffset + 1);
            indices->push_back(last);
        }
        else {
            indices->push_back(lroffset + j + 1);
            indices->push_back(lroffset + j + 2);
            indices->push_back(last);
        }
    }

    for (int i = 0; i < positions->size(); i++) {
        normals->push_back(positions->at(i));
    }
}

