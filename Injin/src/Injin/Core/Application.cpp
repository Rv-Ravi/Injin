#include "Application.h"
#include <stdio.h>



engin::Application::Application(winProperties winProp)
{
	m_winGl = std::make_unique<WindowGL>(winProp);
	engin::grpicInit();
	m_winGl->setCallbacks();

	m_guiLayer = std::make_unique<engin::ImguiLayer>(m_winGl->getWindow());
	
}

engin::Application::~Application()
{
	m_guiLayer->imGuiExit();
}

void engin::Application::runApp()
{
	float bgColor[4] = { 0 };

	while (!m_winGl->isWinClose())
	{
		engin::clrColrBuffr(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
		engin::clrBuffr();

		m_guiLayer->setNewFrame();

		ImGui::Begin("Background color");
		ImGui::ColorEdit4("clear color", bgColor);
		ImGui::End();

		m_guiLayer->renderData();

		m_winGl->onUpdate();
	}
}
