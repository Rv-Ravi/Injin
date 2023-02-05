#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <vector>

namespace engin
{
	class ImguiLayer
	{
	public:
		ImguiLayer(GLFWwindow* window);
		~ImguiLayer();

		void setNewFrame();

		void renderData();

		void imGuiExit();
	};

}

#endif //IMGUILAYER_H

