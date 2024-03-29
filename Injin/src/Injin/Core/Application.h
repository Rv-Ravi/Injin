#ifndef APPLICATION_H
#define APPLICATION_H

#include "coreInit.h"
#include "../glContext/graphicsInit.h"
#include "../Renderer/Camera.h"
#include "../ImGui/ImguiLayer.h"
#include <memory>
#include "../Renderer/SceneGraph.h"

#include "TimeStamp.h"
namespace engin {
	class NJIN Application
	{
	private:
		std::unique_ptr <engin::WindowGL> m_winGl;
		std::unique_ptr<engin::ImguiLayer> m_guiLayer;
		std::unique_ptr<engin::TimeStamp> m_deltaTime;

		std::unique_ptr<engin::SceneGraph> m_sceneGraph;

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
