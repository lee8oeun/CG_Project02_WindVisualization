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
	glfwSetKeyCallback(window, key_callback);  //창 닫는 함수 


	glfwSetWindowTitle(window, "Project02");



	//***메인 루프***
	while (!glfwWindowShouldClose(window))
	{

		// Rendering
//		int display_w, display_h;
//		glfwGetFramebufferSize(window, &display_w, &display_h);
//		glViewport(0, 0, display_w, display_h);
		glClearColor(0.2f, 0.2f, 0.2f, 0);  //백그라운드 컬러 설정 0<=r,g,b<=1 
		glClear(GL_COLOR_BUFFER_BIT);


		win->draw(); //1초에 최소 30번 돎. while문 안에 있어서 => draw함수에 버퍼 생성하면 x 낭비니까.  


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();    //이벤트 발생 확인, 발생하면 적절한 callback함수 호출 
		//glfwWaitEvents();



	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}


