//#define  FREEGLUT_LIB_PRAGMAS  0

#include <memory>
#include <iostream>
#include <GL/gl3w.h>
#include <string>
#include "Loader.h"
#include "Viewer.h"


class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	~MyGlWindow() {};

	void draw();
	void setSize(int w, int h) { m_width = w;m_height = h; }

	Viewer* m_viewer;
	
private:

	std::unique_ptr<ShaderProgram> shaderProgram;

	int m_width;
	int m_height;
	
	void setupBuffer();
	void Init();

};
