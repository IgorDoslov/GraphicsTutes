#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Input.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update(float deltaTime);

	void SetPosition(const glm::vec3 &pos);
	void SetYawPitchRoll(float yaw, float pitch, float roll);
	void SetProjection(float fov, float aspect, float near, float far);

	glm::vec3& GetPosition();

	float GetYaw();
	float GetPitch();
	float GetRoll();

	glm::mat4& GetView();
	glm::mat4& GetProjection();

	void Lookat(glm::vec3 target);

protected:
	glm::vec3 m_camPosition = glm::vec3(0, 2, -2);
	glm::vec3 m_camfront = glm::vec3(0, 0, 1);
	glm::vec3 m_camUp = glm::vec3(0, 1, 0);

	float m_camYaw = 0.0f;
	float m_camPitch = 0.0f;
	float m_camRoll = 0.0f;


	float m_camMoveSpeed = 10.0f;

	int m_lastMouseXPos, m_lastMouseYPos;

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
private:
};

