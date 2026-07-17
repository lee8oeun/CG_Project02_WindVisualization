//#define  FREEGLUT_LIB_PRAGMAS  0

#include <memory>
#include <iostream>
#include <GL/gl3w.h>
#include <string>
#include "../shaders/Program.h"
#include "Viewer.h"
#include "Wind.h"
#include "Sphere.h"


class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	~MyGlWindow() {};

	void draw();
	void setSize(int w, int h) { m_width = w;m_height = h; }

	Viewer* m_viewer;
	Wind* m_particle;
	Sphere* m_sphere;
private:

	std::unique_ptr<Program> shaderProgram;
	std::unique_ptr<Program> ParticleShader;

	int m_width;
	int m_height;

	void setupBuffer();
	void Init();

};
