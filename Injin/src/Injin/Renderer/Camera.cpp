#include "Camera.h"
#include "../ImGui/ImguiLayer.h"
#include <iostream>

glm::vec3 engin::lerp(float alpha, const glm::vec3& a, const glm::vec3& b)
{
	return { (b.x - a.x) * alpha + a.x, (b.y - a.y) * alpha + a.y, (b.z - a.z) * alpha + a.z };
}


engin::Camera::Camera()
	:m_camPos(0.f,0.f,10.f), m_camMaxVel(6.f),m_viewMat(1.f),m_projMat(1.f),
	m_viewProjMat(1.f), m_zNear(0.01f), m_zFar(100.f), m_width(16.f), m_height(9.f)
{
}

engin::Camera::Camera(const glm::vec3& camPos)
	:m_camPos(camPos), m_camMaxVel(6.f), m_viewMat(1.f), m_projMat(1.f),
	m_viewProjMat(1.f), m_zNear(0.01f), m_zFar(100.f), m_width(16.f), m_height(9.f)
{
}


engin::PerspectiveCamera::PerspectiveCamera(const glm::vec3& position)
	:Camera(position), m_pitch(0.f), m_yaw(0.f), m_lastX(0.f), m_lastY(0.f),m_fov(45.f),
	m_camX(1.f, 0.f, 0.f), m_camY(0.f, 0.f, -1.f), m_camZ(0.f, 1.f, 0.f), m_camDir(-m_camZ)
{
	showFront();
	cameraCalculation();
}

engin::PerspectiveCamera::~PerspectiveCamera()
{
}

glm::vec3 engin::PerspectiveCamera::getCamDir()
{
	return m_camDir;
}

void engin::PerspectiveCamera::rotateCam(const WindowGL& window, float dtime)
{
	
	if (window.getButtonState(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
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

void engin::PerspectiveCamera::mvCam(const WindowGL& window, float dtime)
{
	rotateCam(window, dtime);
	glfwGetFramebufferSize(window.getWindow(), (int*)&m_width, (int*)&m_height);
	if (window.getKeyState(GLFW_KEY_RIGHT_SHIFT) == 1 && window.getKeyState(GLFW_KEY_UP) == 1)
		m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * m_camY);
	else if (window.getKeyState(GLFW_KEY_RIGHT_SHIFT) == 1 && window.getKeyState(GLFW_KEY_DOWN) == 1)
		m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * -m_camY);
	else if (window.getKeyState(GLFW_KEY_UP) == 1)
		m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * -m_camZ);
	else if (window.getKeyState(GLFW_KEY_DOWN) == 1)
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

void engin::PerspectiveCamera::imGuiWindows()
{
	ImGui::Text("Position : %.2f, %.2f, %.2f", m_camPos.x, m_camPos.y, m_camPos.z);

	ImGui::Text("Camera X Direction : %.2f, %.2f, %.2f", m_camX.x, m_camX.y, m_camX.z);
	ImGui::Text("Camera Y Direction : %.2f, %.2f, %.2f", m_camY.x, m_camY.y, m_camY.z);
	ImGui::Text("Camera Z Direction : %.2f, %.2f, %.2f", m_camZ.x, m_camZ.y, m_camZ.z);

	ImGui::DragFloat("Field Of View", &m_fov, 1.f, 10.f, 60.f, "%.2f");
	ImGui::DragFloat3("Camera Velocity", &m_camMaxVel.x, 1.f, 1.f, 60.f, "%.2f");
	ImGui::DragFloat("Camera Near", &m_zNear, 1.f, 0.01f, 5.f, "%.2f");
	ImGui::DragFloat("Camera Far", &m_zFar, 1.f, 1.f, 100.f, "%.2f");

	if (ImGui::Button("Perspective"))
		showIsometric();
	if (ImGui::Button("Right"))
		showRight();
	ImGui::SameLine();
	if (ImGui::Button("Left"))
		showLeft();
	else if (ImGui::Button("Front"))
		showFront();
	ImGui::SameLine();
	if (ImGui::Button("Back"))
		showBack();
}

void engin::PerspectiveCamera::showTop()
{
	m_camPos = glm::vec3(0.f, 20.f, 0.f);
	m_pitch = 0.f; m_yaw = 180.f;
	m_camZ = glm::vec3(0.f, 1.f, 0.f);
	m_camY = glm::vec3(0.f, 0.f, -1.f);
	m_camX = glm::vec3(1.f, 0.f, 0.f);
}

void engin::PerspectiveCamera::showBottom()
{
	m_camPos = glm::vec3(0.f, -20.f, 0.f);
	m_camZ = glm::vec3(0.f, -1.f, 0.f);
	m_camY = glm::vec3(0.f, 0.f, 1.f);
	m_camX = glm::vec3(1.f, 0.f, 0.f);
	m_pitch = 270.f; m_yaw = 0.f;
}

void engin::PerspectiveCamera::showRight()
{
	m_camPos = glm::vec3(20.f, 0.f, 0.f);
	m_camZ = glm::vec3(1.f, 0.f, 0.f);
	m_camY = glm::vec3(0.f, 1.f, 0.f);
	m_camX = glm::vec3(0.f, 0.f, -1.f);
	m_pitch = 0.f; m_yaw = 90.f;
}

void engin::PerspectiveCamera::showLeft()
{
	m_camPos = glm::vec3(-20.f, 0.f, 0.f);
	m_camZ = glm::vec3(-1.f, 0.f, 0.f);
	m_camY = glm::vec3(0.f, 1.f, 0.f);
	m_camX = glm::vec3(0.f, 0.f, 1.f);
	m_pitch = 0.f; m_yaw = 270.f;
}

void engin::PerspectiveCamera::showFront()
{
	m_camPos = glm::vec3(0.f, 0.f, 20.f);
	m_camZ = glm::vec3(0.f, 0.f, 1.f);
	m_camY = glm::vec3(0.f, 1.f, 0.f);
	m_camX = glm::vec3(1.f, 0.f, 0.f);
	m_pitch = 0.f; m_yaw = 0.f;
}

void engin::PerspectiveCamera::showBack()
{
	m_camPos = glm::vec3(0.f, 0.f, -20.f);
	m_camZ = glm::vec3(0.f, 0.f, -1.f);
	m_camY = glm::vec3(0.f, 1.f, 0.f);
	m_camX = glm::vec3(-1.f, 0.f, 0.f);
	m_pitch = 0.f; m_yaw = 180.f;
}

void engin::PerspectiveCamera::showIsometric()
{
	m_pitch = 30.f; m_yaw = 0.0f;
	m_camZ.x = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	m_camZ.y = sin(glm::radians(m_pitch));
	m_camZ.z = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));

	m_camPos = glm::normalize(m_camZ) * 10.f;
}

void engin::PerspectiveCamera::cameraCalculation()
{
	calcViewMat();
	calcProjMat();
	calcViewProjMat();
}

void engin::PerspectiveCamera::calcViewMat()
{

	m_camZ = glm::normalize(m_camZ);
	m_camX = glm::normalize(glm::cross(m_camY, m_camZ));
	m_camY = glm::cross(m_camZ, m_camX);
	m_camDir = -m_camZ;


	m_viewMat[0][0] = m_camX.x; m_viewMat[1][0] = m_camX.y; m_viewMat[2][0] = m_camX.z;
	m_viewMat[0][1] = m_camY.x; m_viewMat[1][1] = m_camY.y; m_viewMat[2][1] = m_camY.z;
	m_viewMat[0][2] = m_camZ.x; m_viewMat[1][2] = m_camZ.y; m_viewMat[2][2] = m_camZ.z;

	m_viewMat[3][0] = -glm::dot(m_camX, m_camPos);
	m_viewMat[3][1] = -glm::dot(m_camY, m_camPos);
	m_viewMat[3][2] = -glm::dot(m_camZ, m_camPos);
}

void engin::PerspectiveCamera::calcProjMat()
{
	m_projMat = glm::perspective(glm::radians(m_fov), m_width / m_height, m_zNear, m_zFar);
}

void engin::PerspectiveCamera::calcViewProjMat()
{
	m_viewProjMat = m_projMat * m_viewMat;
}
