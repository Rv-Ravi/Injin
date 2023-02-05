#include "graphicsInit.h"
#include "../ImGui/ImguiLayer.h"


uint16_t engin::drawMode = GL_TRIANGLES;
glm::vec4 engin::grpicsInit::bgColor(0.f);


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

void engin::grpicsInit::clrColrBuffr()
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

void engin::grpicsInit::imGuiBg()
{
	ImGui::Begin("Background color");
	ImGui::ColorEdit4("clear color", &bgColor.x);
	ImGui::End();
}
