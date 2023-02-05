#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <GLFW/glfw3.h>

namespace engin {

	class TimeStamp
	{
	private:
		float m_prevTime = 0.f, m_framTime = 0.f, m_FPS = 0.f;

	public:

		TimeStamp() = default;
		~TimeStamp() {}

		//methods
	public:
		float getframTime() const { return m_framTime; }
		float getFPS() const { return m_FPS; }

		void update() {
			float currentFramTime = (float)glfwGetTime();
			m_framTime = currentFramTime - m_prevTime;
			m_prevTime = currentFramTime;

			m_FPS = 1.f / m_framTime;
		}
	};
}





#endif //TIMESTAMP_H
