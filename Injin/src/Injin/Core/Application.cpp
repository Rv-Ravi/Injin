#include "Application.h"
#include "../Renderer/Meshes.h"
#include <stdio.h>



engin::Application::Application(winProperties winProp)
{
	m_winGl = std::make_unique<WindowGL>(winProp);
	engin::grpicsInit::grpicInit();
	m_winGl->setCallbacks();

	m_guiLayer = std::make_unique<engin::ImguiLayer>(m_winGl->getWindow());
	
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
		engin::grpicsInit::clrColrBuffr();
		engin::grpicsInit::clrBuffr();

		m_guiLayer->setNewFrame();

		engin::grpicsInit::imGuiBg();

		mesh_1.drawMesh();

		m_guiLayer->renderData();

		m_winGl->onUpdate();
	}
}
