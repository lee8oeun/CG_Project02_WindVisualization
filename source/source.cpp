#define GLFW_INCLUDE_GLU

#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "MyGlWindow.h"

std::unique_ptr<MyGlWindow> win;

bool lbutton_down;
bool rbutton_down;
bool mbutton_down;
double m_lastMouseX;
double m_lastMouseY;
double cx, cy;

const unsigned int width = 800;
const unsigned int height = 600;

//height mat
int useWireframe = 0;
int displayGrayscale = 0;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}


static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	cx = xpos;
	cy = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_lastMouseX = xpos;
		m_lastMouseY = ypos;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			lbutton_down = true;
		else if (GLFW_RELEASE == action)
			lbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (GLFW_PRESS == action)
			rbutton_down = true;
		else if (GLFW_RELEASE == action)
			rbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (GLFW_PRESS == action)
			mbutton_down = true;
		else if (GLFW_RELEASE == action)
			mbutton_down = false;
	}
}


void mouseDragging(double width, double height)
{

	if (lbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);


		win->m_viewer->rotate(fractionChangeX, fractionChangeY);
	}
	else if (mbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win->m_viewer->zoom(fractionChangeY);
	}
	else if (rbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win->m_viewer->translate(-fractionChangeX, -fractionChangeY, 1);
	}



	m_lastMouseX = cx;
	m_lastMouseY = cy;

}

int main(void)
{


	GLFWwindow* window;  //검은 창 생성

	//glfw init -> gl3w init
	/* Initialize the library */
	if (!glfwInit())     //맨처음, 초기화 작업 
	{
		std::cout << "glfw init error"<<std::endl;   //에러 뜨면 이거 출력
		return 0;
	}

	//어떤 버전 쓸지 결정 : 4.3 사용 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//modern 오픈지엘 사용 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//검은 창 사이즈 
	

	

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);


	if (!window)
	{
		//만약 에러가 있으면 스탑 
		glfwTerminate();
		return -1;
	}


	//중요! OPENGL countext= 오픈지엘을 사용하기 위한 거대한 구조체 
	glfwMakeContextCurrent(window);

	//오픈지엘 헬퍼 함수 -- 외부 라이브러리 사용 (오픈지엘을 호출하기 위한 함수, 무조건 필요_) 
	/* Make the window's context current */
	if (gl3wInit()) {
		// 		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}

	//	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	// 
	// 	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
	// 		glGetString(GL_SHADING_LANGUAGE_VERSION));


	glfwSwapInterval(1);  //enable vsync

	// Setup ImGui binding



	win = std::make_unique<MyGlWindow>(width, height);

	bool show_test_window = true;
	bool show_another_window = false;



	double previousTime = glfwGetTime();
	int frameCount = 0;
	double lastTime = 0;

	//콜백 함수정의 

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetKeyCallback(window, key_callback);


	glfwSetWindowTitle(window, "Project02");
	//***메인 루프***
	while (!glfwWindowShouldClose(window))
	{

		// Rendering


		glClearColor(0.3f, 0.3f, .3f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glEnable(GL_DEPTH_TEST);

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);


		win->draw();   //그리기 !! 

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		//glfwWaitEvents();

		mouseDragging(display_w, display_h);

	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}


