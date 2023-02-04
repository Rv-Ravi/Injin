#ifndef SAMPLE_H
#define SAMPLE_H

#include "coreInit.h"
#include "../glContext/graphicsInit.h"
#include "../Windows/WindowGL.h"
#include "../ImGui/ImguiLayer.h"
#include <memory>
namespace engin {
	class NJIN Application
	{
	private:
		std::unique_ptr <engin::WindowGL> m_winGl;
		std::unique_ptr<engin::ImguiLayer> m_guiLayer;

	public:
		Application(winProperties winProp = winProperties());
		virtual ~Application();

		//methods
		void runApp();
	};
}

#endif //SAMPLE_H
