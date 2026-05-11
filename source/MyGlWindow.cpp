
#define GLM_ENABLE_EXPERIMENTAL 
#include "MyGlWindow.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

static float DEFAULT_VIEW_POINT[3] = { 67, 627, 169 };  //카메라 위치
static float DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };  //카메라가 바라보는 지점
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };  //카메라 위쪽 방향


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

void MyGlWindow::Init()
{
	glEnable(GL_DEPTH_TEST);
}

void MyGlWindow::setupBuffer()
{
	shaderProgram = std::make_unique<ShaderProgram>();
	shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");


	shaderProgram->addUniform("model");
	shaderProgram->addUniform("view");
	shaderProgram->addUniform("projection");
}
void MyGlWindow::draw(void)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram->use();

	// MVP 행렬 계산
	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
		(float)m_width / m_height, 0.1f, 100000.0f);
	glm::vec3 eye = m_viewer->getViewPoint();
	glm::vec3 look = m_viewer->getViewCenter();
	glm::vec3 up = m_viewer->getUpVector();
	glm::mat4 view = lookAt(eye, look, up);
	
	glm::mat4 model = glm::mat4(1.0f);

	glUniformMatrix4fv(shaderProgram->uniform("projection"),
		1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(shaderProgram->uniform("view"),
		1, GL_FALSE, glm::value_ptr(view));

	glUniformMatrix4fv(shaderProgram->uniform("model"),
		1, GL_FALSE, glm::value_ptr(model));
	

	shaderProgram->disable();
}
