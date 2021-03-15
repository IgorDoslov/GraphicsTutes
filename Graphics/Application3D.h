#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

class Camera;

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void DrawGizmosGrid();

protected:

	Camera* m_camera;
};