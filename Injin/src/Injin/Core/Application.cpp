#include "Application.h"
#include <stdio.h>



engin::Application::Application(winProperties winProp)
{
	m_winGl = std::make_unique<WindowGL>(winProp);
}

engin::Application::~Application()
{
}

void engin::Application::runApp()
{

	while (!m_winGl->isWinClose())
	{
		m_winGl->onUpdate();
	}
}
