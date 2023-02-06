#include "graphicsInit.h"
#include "../ImGui/ImguiLayer.h"


uint16_t engin::drawMode = GL_TRIANGLES;



engin::grpicsInit::grpicsInit()
{
	
}

engin::grpicsInit::~grpicsInit()
{
}

void engin::grpicsInit::clrBuffr()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void engin::grpicsInit::clrColrBuffr(const glm::vec4& bgColor)
{
	glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.a);
}

void engin::grpicsInit::grpicInit()
{
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		WindowGL::terminate();
		exit(1);
	}
}

