#pragma once

#include <GL/gl3w.h>
#include <vector>
#include <Compute.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using namespace std;

//입자 위치 저장
struct Particle {
	float position[4]; //위치 xyz + 수명 
	float velocity[4]; //xyz의 속도 + 패딩(일부로 비워둠)
};

class WindParticles {
public:
	WindParticles();

	void initialize();
	void update(float dt); //compute shader 실행 
	void draw();

private:
	
	const int NUM_PARTICLES = 10240;

	vector<glm::vec3> windPositions;
	vector<Particle> particles;

	GLuint vao;
	GLuint ssbo; //vbo 대신 ssbo

	ComputeShader* c_shader;

};