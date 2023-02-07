#include "Camera.h"
#include "../ImGui/ImguiLayer.h"

namespace engin
{
	glm::vec4 Camera::bgColor(0.6823f,0.9607f,0.9607f,1.f);
	glm::vec3 lerp(float alpha, const glm::vec3& a, const glm::vec3& b)
	{
		return { (b.x - a.x) * alpha + a.x, (b.y - a.y) * alpha + a.y, (b.z - a.z) * alpha + a.z };
	}


	Camera::Camera()
		:m_camPos(0.f,0.f,1.f), m_OrthocamPos(0.f,0.f,1.f), m_camX(1.f, 0.f, 0.f), m_camY(0.f, 1.f, 0.f), m_camZ(0.f, 0.f, 1.f), m_camDir(-m_camZ),
		m_width(16.f),m_height(9.f), m_pitch(0.f), m_yaw(0.f), m_lastX(0.f), m_lastY(0.f),
		m_viewMat(1.f), m_projMat(1.f), m_viewProjMat(1.f), m_camSpeed(0.f), m_zNear(0.01f), m_zFar(50.f), m_fov(45)
	{
		
		calcViewMat();
		calcProjMat();
		calcViewProjMat();

	}

	Camera::Camera(const glm::vec3& camPos, bool isOrhto)
		:m_camPos(camPos),m_OrthocamPos(0.f,0.f,1.f), m_camX(1.f, 0.f, 0.f), m_camY(0.f, 1.f, 0.f), m_camZ(0.f, 0.f, 1.f), m_camDir(-m_camZ),
		m_width(16.f), m_height(9.f), m_pitch(0.f), m_yaw(0.f), m_lastX(0.f), m_lastY(0.f),
		m_viewMat(1.f), m_projMat(1.f), m_viewProjMat(1.f), m_camSpeed(0.f), m_zNear(0.01f), m_zFar(50.f), m_fov(45)
	{
		this->isOrtho = isOrhto;
		calcViewMat();
		calcProjMat();
		calcViewProjMat();
	}


	Camera::~Camera()
	{
	}

	void Camera::calcViewMat() {
		if (m_camZ != -m_camDir)
		{
			m_camZ = glm::normalize(m_camZ);
			m_camX = glm::normalize(glm::cross(m_camY, m_camZ));
			m_camY = glm::cross(m_camZ, m_camX);
			m_camDir = -m_camZ;
		}

		m_viewMat[0][0] = m_camX.x; m_viewMat[1][0] = m_camX.y; m_viewMat[2][0] = m_camX.z;
		m_viewMat[0][1] = m_camY.x; m_viewMat[1][1] = m_camY.y; m_viewMat[2][1] = m_camY.z;
		m_viewMat[0][2] = m_camZ.x; m_viewMat[1][2] = m_camZ.y; m_viewMat[2][2] = m_camZ.z;
		if (isOrtho)
		{
			m_viewMat[3][0] = -glm::dot(m_camX, m_OrthocamPos);
			m_viewMat[3][1] = -glm::dot(m_camY, m_OrthocamPos);
			m_viewMat[3][2] = -glm::dot(m_camZ, m_OrthocamPos);
		}
		else {
			m_viewMat[3][0] = -glm::dot(m_camX, m_camPos);
			m_viewMat[3][1] = -glm::dot(m_camY, m_camPos);
			m_viewMat[3][2] = -glm::dot(m_camZ, m_camPos);
		}


	}

	void Camera::calcProjMat() {
		if (isOrtho)
			m_projMat = glm::ortho(-4.f, 4.f, -3.f, 3.f, m_zNear, m_zFar);
		else
			m_projMat = glm::perspective(glm::radians(m_fov), m_width / m_height, m_zNear, m_zFar);
		calcViewProjMat();
	}

	void Camera::setPos(const glm::vec3& pos)
	{
		m_camPos.x = pos.x;
		m_camPos.y = pos.y;
		m_camPos.z = pos.z;
	}

	glm::vec3 Camera::getPos()
	{
		return m_camPos;
	}

	void Camera::calcViewProjMat()
	{
		m_viewProjMat = m_projMat * m_viewMat;
	}

	glm::vec3 Camera::getCamDir()
	{
		return m_camDir;
	}
	void Camera::mvCam(const WindowGL& window, float dtime)
	{
		glfwGetWindowSize(window.getWindow(),(int*) & m_width, (int*)&m_height);
		rotateCam(window, dtime);
		if (!isOrtho)
			mvCamSmth(window, dtime);
		else
			mvCmNorm(window, dtime);
		calcViewMat();
		calcProjMat();
	}
	void Camera::imGuiWindows()
	{
		ImGui::Begin("Main Camera settings");
		if (isOrtho)
		{
			ImGui::Text("Position : %.2f, %.2f, %.2f", m_OrthocamPos.x, m_OrthocamPos.y, m_OrthocamPos.z);
		}
		else {
			ImGui::Text("Position : %.2f, %.2f, %.2f", m_camPos.x, m_camPos.y, m_camPos.z);
		}
		
		ImGui::Text("Camera X Direction : %.2f, %.2f, %.2f", m_camX.x, m_camX.y, m_camX.z);
		ImGui::Text("Camera X Direction : %.2f, %.2f, %.2f", m_camY.x, m_camY.y, m_camY.z);
		ImGui::Text("Camera X Direction : %.2f, %.2f, %.2f", m_camZ.x, m_camZ.y, m_camZ.z);

		ImGui::ColorEdit4("clear color", &engin::Camera::bgColor.x);
		ImGui::Checkbox("Orthographic view", &isOrtho);

		ImGui::DragFloat("Field Of View", &m_fov,1.f, 10.f, 60.f, "%.2f");
		ImGui::DragFloat("Camera Near", &m_zNear,1.f, 0.01f, 5.f, "%.2f");
		ImGui::DragFloat("Camera Far", &m_zFar,1.f, 1.f, 100.f, "%.2f");

		ImGui::End();
	}
	void Camera::rotateCam(const WindowGL& window, float dtime)
	{
		if (window.getButtonState(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !this->isOrtho)
		{
			auto curPos = window.getCurPos();

			if (!isMouInit)
			{
				m_lastX = (float)curPos[0];
				m_lastY = (float)curPos[1];
				isMouInit = true;
			}

			m_yaw += (m_lastX - (float)curPos[0]) * dtime * 4.f;
			m_pitch += ((float)curPos[1] - m_lastY) * dtime * 4.f;

			m_lastY = (float)curPos[1];
			m_lastX = (float)curPos[0];

			m_camZ.x = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
			m_camZ.y = sin(glm::radians(m_pitch));
			m_camZ.z = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));

		}
		else {
			isMouInit = false;
		}
	}
	void Camera::mvCamSmth(const WindowGL& window, float dtime)
	{
		if (window.getKeyState(GLFW_KEY_RIGHT_SHIFT) == 1 && window.getKeyState(GLFW_KEY_UP) == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * m_camY);
		else if (window.getKeyState(GLFW_KEY_RIGHT_SHIFT) == 1 && window.getKeyState(GLFW_KEY_DOWN) == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * -m_camY);
		else if (!isOrtho && window.getKeyState(GLFW_KEY_UP) == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * -m_camZ);
		else if (!isOrtho && window.getKeyState(GLFW_KEY_DOWN) == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * m_camZ);
		else
			m_camSpeed = lerp(dtime, m_camSpeed, 0.f * m_camZ);


		if (window.getKeyState(GLFW_KEY_RIGHT) == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * m_camX);
		else if (window.getKeyState(GLFW_KEY_LEFT) == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * -m_camX);
		else
			m_camSpeed = lerp(dtime, m_camSpeed, 0.f * m_camX);


		m_camPos += m_camSpeed * dtime;
	}
	void Camera::mvCmNorm(const WindowGL& window, float dtime)
	{
		if (window.getKeyState(GLFW_KEY_RIGHT_SHIFT) == 1 && window.getKeyState(GLFW_KEY_UP) == 1)
			m_OrthocamPos.y += m_camMaxVel.y * dtime;
		else if (window.getKeyState(GLFW_KEY_RIGHT_SHIFT) == 1 && window.getKeyState(GLFW_KEY_DOWN) == 1)
			m_OrthocamPos.y -= m_camMaxVel.y * dtime;
		else if (!isOrtho && window.getKeyState(GLFW_KEY_UP) == 1)
			m_OrthocamPos.z -= m_camMaxVel.z * dtime;
		else if (!isOrtho && window.getKeyState(GLFW_KEY_DOWN) == 1)
			m_OrthocamPos.z += m_camMaxVel.z * dtime;

		if (window.getKeyState(GLFW_KEY_RIGHT) == 1)
			m_OrthocamPos.x += m_camMaxVel.x * dtime;
		else if (window.getKeyState(GLFW_KEY_LEFT) == 1)
			m_OrthocamPos.x -= m_camMaxVel.x * dtime;
	}
}
