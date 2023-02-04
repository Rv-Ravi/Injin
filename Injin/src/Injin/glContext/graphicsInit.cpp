#include "graphicsInit.h"

void engin::clrBuffr()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void engin::clrColrBuffr(float x, float y, float z, float a)
{
	glClearColor(x, y, z, a);
}

void engin::grpicInit()
{
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		WindowGL::terminate();
		exit(1);
	}
}
