#pragma once

#include <GL/gl3w.h>
#include <vector>
#include <Compute.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using namespace std;


struct Particle {
	float position[3]; //위치 xyz
};

class Wind {
public:
	Wind();

	void initialize();
	void draw();

private:
	vector<Particle> particles;

	GLuint vao, ssbo;
	ComputeShader* c_shader;

};
