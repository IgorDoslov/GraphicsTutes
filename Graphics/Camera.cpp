#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();


	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_camPosition += m_camfront * m_camMoveSpeed * deltaTime;
	}

	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_camPosition -= m_camfront * m_camMoveSpeed * deltaTime;
	}

	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		glm::vec3 dir = glm::normalize(glm::cross(m_camfront, m_camUp));
		m_camPosition -= dir * m_camMoveSpeed * deltaTime;
	}

	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		glm::vec3 dir = glm::normalize(glm::cross(m_camfront, m_camUp));
		m_camPosition += dir * m_camMoveSpeed * deltaTime;
	}


	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		int mouseXPos, mouseYPos;
		input->getMouseXY(&mouseXPos, &mouseYPos);

		float deltaMouseX = mouseXPos - m_lastMouseXPos;
		float deltaMouseY = mouseYPos - m_lastMouseYPos;

		m_camYaw += deltaMouseX;
		m_camPitch += deltaMouseY;

		if (m_camPitch > 85.0f) m_camPitch = 85.0f;
		if (m_camPitch < -85.0f) m_camPitch = -85.0f;


		glm::vec3 front;
		front.x = glm::cos(glm::radians(m_camYaw)) * glm::cos(glm::radians(m_camPitch));
		front.y = glm::sin(glm::radians(m_camPitch));
		front.z = glm::sin(glm::radians(m_camYaw)) * glm::cos(glm::radians(m_camPitch));
		m_camfront = glm::normalize(front);
	}


	m_viewMatrix = glm::lookAt(m_camPosition, m_camPosition + m_camfront, m_camUp);

	input->getMouseXY(&m_lastMouseXPos, &m_lastMouseYPos);

}

void Camera::SetPosition(const glm::vec3 &pos)
{
	m_camPosition = pos;
}

void Camera::SetYawPitchRoll(float yaw, float pitch, float roll)
{
	m_camYaw = yaw;
	m_camPitch = pitch;
	m_camRoll = roll;
}

void Camera::SetProjection(float fov, float aspect, float near, float far)
{
	m_projectionMatrix = glm::perspective(fov, aspect, near, far);
}

glm::vec3& Camera::GetPosition()
{
	return m_camPosition;
}

float Camera::GetYaw()
{
	return m_camYaw;
}

float Camera::GetPitch()
{
	return m_camPitch;
}

float Camera::GetRoll()
{
	return m_camRoll;
}

glm::mat4& Camera::GetView()
{
	return m_viewMatrix;
}

glm::mat4& Camera::GetProjection()
{
	return m_projectionMatrix;
}

void Camera::Lookat(glm::vec3 target)
{
	glm::vec3 dir = glm::normalize(target - m_camPosition);
	m_camPitch = glm::degrees(glm::asin(dir.y));
	m_camYaw = glm::degrees(atan2(dir.y, dir.x));
}
