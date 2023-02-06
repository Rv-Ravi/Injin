#ifndef WINDOWGL_H
#define WINDOWGL_H


#include <GLFW/glfw3.h>
#include <array>

namespace engin
{
	struct winProperties
	{
		int32_t m_width, m_height;
		const char* m_title;
		bool m_isSync = true;
		

		winProperties(int32_t width = 1080, int32_t height = 720, const char* name = "Editor Application")
			:m_width(width),m_height(height),m_title(name)
		{}
	};

	class WindowGL
	{
	private:
		GLFWwindow* m_window;
	public:
		//constructor
		WindowGL(winProperties& winProp);
		~WindowGL();

		//getter and setter
		GLFWwindow* getWindow()const  { return m_window; }

		//methods

		void onUpdate();
		void destryWin();
		static void terminate();

		uint16_t getKeyState(uint16_t key) const;
		uint16_t getButtonState(uint16_t button) const;
		std::array<double,2> getCurPos() const;

		bool isWinClose() const;
		void setCallbacks();

	private:
		void Init(winProperties& winProp);
		


	};
}


#endif //WINDOWGL_H
