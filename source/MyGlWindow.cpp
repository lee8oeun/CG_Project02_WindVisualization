
#define GLM_ENABLE_EXPERIMENTAL 
#include "MyGlWindow.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

static float DEFAULT_VIEW_POINT[3] = {0,0,5 };  //카메라 위치
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };  //카메라가 바라보는 지점
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };  //카메라 위쪽 방향

std::unique_ptr<Program> program = nullptr;

MyGlWindow::MyGlWindow(int w, int h)
//==========================================================================
{
	m_width = w;
	m_height = h;


	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h); //화면 가로/세로 비율

	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

	Init();
	setupBuffer();
}

glm::mat4 lookAt(glm::vec3 pos, glm::vec3 look, glm::vec3 up)
{

	glm::vec3 zaxis = glm::normalize(pos - look);
	glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

	glm::mat4 R;

	R[0] = glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0.0f);
	R[1] = glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0.0f);
	R[2] = glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0.0f);
	R[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 T;

	T[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	T[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	T[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	T[3] = glm::vec4(-pos.x, -pos.y, -pos.z, 1.0f);


	return R * T;
}

glm::mat4 perspective(float fov, float aspect, float n, float f)//(매개변수 :)시야각, 종횡비, 카메라가 볼 수 있는 최소거리, 최대거리 
{
	glm::mat4 P(0.0f);

	const float tanHalfFOV = tan(glm::radians(fov) / 2.0f);

	float A = (-f - n) / (f - n);
	float B = (2 * (n * f)) / (n - f);

	P[0] = glm::vec4(1.0f / (aspect * tanHalfFOV), 0, 0, 0);
	P[1] = glm::vec4(0, 1.0 / tanHalfFOV, 0, 0);
	P[2] = glm::vec4(0, 0, A, -1.0f);
	P[3] = glm::vec4(0, 0, B, 0.0f);

	return P;
}

void MyGlWindow::Init()
{
	glEnable(GL_DEPTH_TEST);
}

void MyGlWindow::setupBuffer()
{
	//new shader
	try {
		shaderProgram = std::unique_ptr<Program>(Program::GenerateFromFileVsFs("shaders/simple.vert", "shaders/simple.frag"));
		ParticleShader = std::unique_ptr<Program>(Program::GenerateFromFileVsFs("shaders/particle.vert", "shaders/particle.frag"));
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Shader Error : " << e.what() << std::endl;
		std::cerr << "Failed to load shader" << std::endl;
		exit(1);
	}
	m_particle = new WindParticles();
	m_particle->initialize();

	m_sphere = new Sphere(1.0f,36,18);
}

void MyGlWindow::draw(void)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram->BindProgram();

	//model
	glm::mat4 modelSphere(1.0f);
	modelSphere = glm::translate(modelSphere, glm::vec3(0.0f, 0.0f, 0.0f));
	modelSphere = glm::rotate(modelSphere, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//view
	glm::vec3 camPos = m_viewer->getViewPoint();
	glm::vec3 camLook = m_viewer->getViewCenter();
	glm::vec3 upVector = m_viewer->getUpVector();
	glm::mat4 view = glm::lookAt(camPos, camLook, upVector);
	//projection
	glm::mat4 projection = perspective(45.0f, 1.0f * m_width / m_height, 0.1f, 500.0f);
	//mvp
	glm::mat4 mvpSphere = projection * view * modelSphere;
	shaderProgram->SetMatrix("mvp", mvpSphere);

	shaderProgram->SetTexture("Tex1",0);

	m_sphere->draw();

	shaderProgram->UnbindProgram();

	m_particle->update(0.016f);  // 추가: compute shader 실행
	ParticleShader->BindProgram();
	ParticleShader->SetMatrix("mvp", mvpSphere);

	m_particle->draw();

	ParticleShader->UnbindProgram();

}
