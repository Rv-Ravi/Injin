#include "Application.h"
#include "../Renderer/Meshes.h"
#include <stdio.h>
#include "../Renderer/ShaderProgram.h"


engin::Application::Application(winProperties winProp)
{
	m_winGl = std::make_unique<WindowGL>(winProp);
	engin::grpicsInit::grpicInit();
	m_winGl->setCallbacks();

	m_guiLayer = std::make_unique<engin::ImguiLayer>(m_winGl->getWindow());
	m_deltaTime = std::make_unique<engin::TimeStamp>();
	m_camera = std::make_unique<engin::Camera>(glm::vec3( 0.f,0.f,3.f ));
}

engin::Application::~Application()
{
	m_guiLayer->imGuiExit();
}

void engin::Application::runApp()
{
	engin::ShaderProgram program("D:\\Coding\\GameEngine\\Injin\\Editor\\assets\\Shaders\\simpleObj.glsl");
	
	glm::vec3 triColor = { 0.3f,0.f,0.f };

	program.bindProgram();
	program.setUniValuefV("uColor", triColor, 3);
	program.setUniValueM("transMat", glm::mat4(1.f), 4);
	
	program.unbindProgram();

	engin::Meshes mesh_1(engin::triangle);

	

	while (!m_winGl->isWinClose())
	{
		this->onUpdate();

		program.bindProgram();
		program.setUniValueM("viewProj", m_camera->getViewProjMat(), 4);
		mesh_1.drawMesh();
		program.unbindProgram();

		this->imGuiUpdates();
		m_camera->mvCam(*m_winGl, m_deltaTime->getframTime());
		m_winGl->onUpdate();
	}
}

void engin::Application::onUpdate()
{
	m_deltaTime->update();
	engin::grpicsInit::clrColrBuffr(engin::Camera::bgColor);
	engin::grpicsInit::clrBuffr();
}

void engin::Application::imGuiUpdates()
{
	m_guiLayer->setNewFrame();

	m_camera->imGuiWindows();

	m_guiLayer->renderData();
}
