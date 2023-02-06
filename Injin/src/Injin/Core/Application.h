#ifndef APPLICATION_H
#define APPLICATION_H

#pragma warning(disable : 4251)

#include "coreInit.h"
#include "../glContext/graphicsInit.h"
#include "../Renderer/Camera.h"
#include "../ImGui/ImguiLayer.h"
#include <memory>
#include "TimeStamp.h"
namespace engin {
	class NJIN Application
	{
	private:
		std::unique_ptr <engin::WindowGL> m_winGl;
		std::unique_ptr<engin::ImguiLayer> m_guiLayer;
		std::unique_ptr<engin::TimeStamp> m_deltaTime;
		std::unique_ptr<engin::Camera> m_camera;

	public:
		Application(winProperties winProp = winProperties());
		virtual ~Application();

		//methods
		void runApp();
		void onUpdate();
		void imGuiUpdates();
	};
}

#endif //SAMPLE_H
