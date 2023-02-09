#include "Application.h"
#include <stdio.h>


engin::Application::Application(winProperties winProp)
{
	m_winGl = std::make_unique<WindowGL>(winProp);
	engin::grpicsInit::grpicInit();
	m_winGl->setCallbacks();

	m_guiLayer = std::make_unique<engin::ImguiLayer>(m_winGl->getWindow());
	m_deltaTime = std::make_unique<engin::TimeStamp>();
	m_sceneGraph = std::make_unique<engin::SceneGraph>();
}

engin::Application::~Application()
{
	m_guiLayer->imGuiExit();
}

void engin::Application::runApp()
{
	while (!m_winGl->isWinClose())
	{
		this->onUpdate();

		m_sceneGraph->drawScene(*m_winGl.get(), m_deltaTime->getframTime());

		this->imGuiUpdates();


		m_sceneGraph->sceneUpdate(*m_winGl.get(),m_deltaTime->getframTime());
		m_winGl->onUpdate();
	}
}

void engin::Application::onUpdate()
{
	m_deltaTime->update();
	engin::grpicsInit::clrColrBuffr({ 1.f,1.f,1.f,1.f });
	engin::grpicsInit::clrBuffr();
}

void engin::Application::imGuiUpdates()
{
	m_guiLayer->setNewFrame();

	m_sceneGraph->ImGuiWindows();

	m_guiLayer->renderData();
}
