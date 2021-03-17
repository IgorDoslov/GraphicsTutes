#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

class Camera;

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;

	static void SetupVertexAttribPointers();
};

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void DrawGizmosGrid();

	void CreateGeometry();
	void DestroyGeometry();

	void LoadShader();
	void UnloadShader();

protected:

	unsigned int m_vertCount;
	unsigned int m_indicesCount;

	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;

	unsigned int m_shader;
	unsigned int m_projectionViewLoc;

	Camera* m_camera;
};