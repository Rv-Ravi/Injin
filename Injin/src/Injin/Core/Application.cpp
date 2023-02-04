#include "Application.h"
#include <stdio.h>



engin::Application::Application(winProperties winProp)
{
	m_winGl = std::make_unique<WindowGL>(winProp);
	engin::grpicInit();
	m_winGl->setCallbacks();
}

engin::Application::~Application()
{
}

void engin::Application::runApp()
{

	while (!m_winGl->isWinClose())
	{
		engin::clrColrBuffr(1.f, 0.f, 0.f, 1.f);
		engin::clrBuffr();

		m_winGl->onUpdate();
	}
}
