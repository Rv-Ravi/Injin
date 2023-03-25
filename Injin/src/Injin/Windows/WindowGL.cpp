#include <glad/glad.h>
#include "WindowGL.h"

#include <iostream>

namespace engin
{
	void getError(int errCode, const char* desc) {

		std::cerr << "Error found in GLFW of error code : [" << errCode << "] says : " << desc << std::endl;

	}
	WindowGL::WindowGL(winProperties& winProp)
	{
		Init(winProp);
		


	}
	WindowGL::~WindowGL()
	{
		destryWin();
	}
	void WindowGL::Init(winProperties& winProp)
	{
		glfwSetErrorCallback(getError);

		if(!glfwInit())
			exit(1);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(winProp.m_width, winProp.m_height, winProp.m_title, nullptr, nullptr);

		if (!m_window)
		{
			glfwTerminate();
			exit(1);
		}

		glfwMakeContextCurrent(m_window);
		winProp.m_isSync ? glfwSwapInterval(1) : glfwSwapInterval(0);
		glfwSetWindowSizeLimits(m_window, 720, 480, 4000, 4000);

	}
	void WindowGL::setCallbacks()
	{

		glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int32_t width, int32_t height) {
				glViewport(0, 0, width, height);
			});

	}
	void WindowGL::onUpdate()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	void WindowGL::destryWin()
	{
		glfwDestroyWindow(m_window);
	}
	void WindowGL::terminate()
	{
		glfwTerminate();
	}
	uint16_t WindowGL::getKeyState(uint16_t key) const 
	{
		return glfwGetKey(m_window,key);
	}
	uint16_t WindowGL::getButtonState(uint16_t button) const 
	{
		return glfwGetMouseButton(m_window,button);
	}
	std::array<double, 2> WindowGL::getCurPos() const 
	{
		double pos[2];
		glfwGetCursorPos(m_window,&pos[0],&pos[1]);
		return { pos[0],pos[1] };
	}
	bool WindowGL::isWinClose() const
	{
		return glfwWindowShouldClose(m_window);
	}
}