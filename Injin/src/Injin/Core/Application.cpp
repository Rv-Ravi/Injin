#include "Application.h"
#include "../Renderer/Meshes.h"
#include <stdio.h>



engin::Application::Application(winProperties winProp)
{
	m_winGl = std::make_unique<WindowGL>(winProp);
	engin::grpicsInit::grpicInit();
	m_winGl->setCallbacks();

	m_guiLayer = std::make_unique<engin::ImguiLayer>(m_winGl->getWindow());
	m_deltaTime = std::make_unique<engin::TimeStamp>();
}

engin::Application::~Application()
{
	m_guiLayer->imGuiExit();
}

void engin::Application::runApp()
{
	engin::Meshes mesh_1(engin::triangle);

	while (!m_winGl->isWinClose())
	{
		this->onUpdate();

		mesh_1.drawMesh();

		this->imGuiUpdates();

		m_winGl->onUpdate();
	}
}

void engin::Application::onUpdate()
{
	m_deltaTime->update();
	engin::grpicsInit::clrColrBuffr();
	engin::grpicsInit::clrBuffr();


}

void engin::Application::imGuiUpdates()
{
	m_guiLayer->setNewFrame();

	engin::grpicsInit::imGuiBg();

	m_guiLayer->renderData();
}
