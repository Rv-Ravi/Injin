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
	protected:
		glm::vec3 m_camPos, m_camMaxVel;
		glm::mat4 m_viewMat, m_projMat, m_viewProjMat;

		float m_zNear, m_zFar, m_width, m_height;

	public:
		Camera();
		Camera(const glm::vec3& camPos);

		virtual ~Camera(){}

		//setters and getters
		inline glm::vec3 getPos()const { return m_camPos; }
		inline glm::vec3 getVelocity()const { return m_camMaxVel; }
		inline float getWidth()const { return m_width; }
		inline float getHeight()const { return m_height; }
		inline float getLength()const { return m_zFar - m_zNear; }

		inline glm::mat4 getViewMat() { return m_viewMat; }
		inline glm::mat4 getProjMat() { return m_projMat; }
		inline glm::mat4 getViewProjMat() { return m_viewProjMat; }

		virtual glm::vec3 getCamDir() = 0;

		virtual void mvCam(const WindowGL& window, float dtime) {}

		virtual void imGuiWindows() {}
		virtual void cameraCalculation() {};

	protected:
		virtual void calcViewMat() {}
		virtual void calcProjMat() {}
		virtual void calcViewProjMat() {}
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(const glm::vec3& position);
		~PerspectiveCamera();


		glm::vec3 getCamDir();

		void rotateCam(const WindowGL& window, float dtime);
		void mvCam(const WindowGL& window, float dtime);

		void imGuiWindows();

		void showTop();
		void showBottom();
		void showRight();
		void showLeft();
		void showFront();
		void showBack();
		void showIsometric();
		void cameraCalculation();

	private:
		void calcViewMat();
		void calcProjMat();
		void calcViewProjMat();

	private:
		glm::vec3 m_camX, m_camY, m_camZ,m_camDir, m_camSpeed = glm::vec3(0.f);
		float m_fov, m_pitch, m_yaw, m_lastX, m_lastY;
		bool isMouInit = false;

	};

	glm::vec3 lerp(float alpha, const glm::vec3& a, const glm::vec3& b);
}

#endif CAMERA_H