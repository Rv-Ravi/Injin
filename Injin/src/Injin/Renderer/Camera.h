#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "../Windows/WindowGL.h"

namespace engin
{
	class Camera
	{
		//data members
	public:
		glm::vec3 m_camPos,m_OrthocamPos, m_camX, m_camY, m_camZ, m_camDir, m_camMaxVel = glm::vec3(6.3f), m_camSpeed;
		glm::mat4 m_viewMat, m_projMat, m_viewProjMat;

		float m_zNear, m_zFar, m_fov, m_width,m_height, m_pitch, m_yaw, m_lastX, m_lastY;
		bool isOrtho = false, isMouInit = false;

		static glm::vec4 bgColor;

	public:
		Camera();
		Camera(const glm::vec3& camPos, bool isOrtho = false);

		~Camera();

		//setters and getters
		void setPos(const glm::vec3& pos);
		glm::vec3 getPos();

		inline glm::mat4 getViewMat() { return m_viewMat; }
		inline glm::mat4 getProjMat() { return m_projMat; }
		inline glm::mat4 getViewProjMat() { return m_viewProjMat; }

		glm::vec3 getCamDir();

		inline void setOrtho(bool val) { isOrtho = val; }
		inline bool getOrtho() { return isOrtho; }
		//methods


		

		void mvCam(const WindowGL& window, float dtime);

		void imGuiWindows();

	private:
		void rotateCam(const WindowGL& window, float dtime);
		void calcViewMat();
		void calcProjMat();
		void calcViewProjMat();
		void mvCamSmth(const WindowGL& window, float dtime);
		void mvCmNorm(const WindowGL& window, float dtime);
	};
}

#endif CAMERA_H