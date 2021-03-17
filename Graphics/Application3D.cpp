#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include <gl_core_4_4.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	m_camera = new Camera();
	m_camera->SetPosition(glm::vec3(10, 10, 10));
	m_camera->Lookat(glm::vec3(0, 0, 0));

	m_camera->SetProjection(glm::radians(45.0f), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	LoadShader();
	CreateGeometry();
	return true;
}

void Application3D::shutdown() {

	DestroyGeometry();
	UnloadShader();
	delete m_camera;
	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {
	m_camera->Update(deltaTime);

	Gizmos::clear();
	// query time since application started
	float time = getTime();

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

}

void Application3D::draw() {
	

	glm::mat4 projection = m_camera->GetProjection();
	glm::mat4 view = m_camera->GetView();

	// wipe the screen to the background colour
	clearScreen();

	// Step 1: Before rendering geometry, tell opengl to use our shader program.
	glUseProgram(m_shader);

	// Step 2: Calculate the projection view matrix
	// and pass it to the shader program (note its a uniform value)
	glm::mat4 projectionView = projection * view;
	//glUniformMatrix4fv(glGetUniformLocation(m_shader, "projectionView"), 1, false, glm::value_ptr(projectionView));
	glUniformMatrix4fv(m_projectionViewLoc, 1, false, &projectionView[0][0]);

	// Step 3: Bind the VAO
	// When we set up the geometry, we did a bunch of glEnableVertexAttribArray and glVertexAttribPointer method calls
	// we also bound the vertex array and index array via the glBindBuffer call.
	// if we were not using VAOs we would have to do those method calls each frame here.
	glBindVertexArray(m_vao);

	// Step 4: Draw Elements. We are using GL_TRIANGLES.
	// we need to tell opengl how many indices there are and the size of our indices
	// when we set up the geometry, our indices were an unsigned char (1 byte of each index)
	glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_BYTE, 0);

	// Step 5: Now that we are done drawing the geometry
	// unbind the vao, we are basically cleaning the opengl state
	glBindVertexArray(0);

	// Step 6: deactivate the shader program, don't do future rendering with it any more.
	glUseProgram(0);

	

	// wipe the gizmos clean for this frame
	//Gizmos::clear();

	// update perspective in case window resized

	// draw 3D gizmos
	DrawGizmosGrid();

	Gizmos::draw(projection * view);
	
	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	//Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}

void Application3D::DrawGizmosGrid()
{


	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));
}

void Application3D::CreateGeometry()
{
	// Step 1:
	// Specify the position and colour for each vert of a cube
	// for this example, each face does not share a vert, so we have four verts for each face of our cube
	Vertex verts[] = {

		// POSITION						COLOUR
		// FRONT FACE					RED
		{{ -0.5f,-0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 0.5f }}, // 0
		{{  0.5f,-0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 0.5f }}, // 1
		{{  0.5f, 0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 0.5f }}, // 2
		{{ -0.5f, 0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 0.5f }}, // 3

		// BACK FACE					YELLOW
		{{ -0.5f,-0.5f,-0.5f, 1.0f },{ 1.0f, 1.0f, 0.0f, 0.5f }}, // 4
		{{  0.5f,-0.5f,-0.5f, 1.0f },{ 1.0f, 1.0f, 0.0f, 0.5f }}, // 5
		{{  0.5f, 0.5f,-0.5f, 1.0f },{ 1.0f, 1.0f, 0.0f, 0.5f }}, // 6
		{{ -0.5f, 0.5f,-0.5f, 1.0f },{ 1.0f, 1.0f, 0.0f, 0.5f }}, // 7

		// LEFT FACE					GREEN
		{{ -0.5f,-0.5f,-0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f }}, // 8
		{{ -0.5f,-0.5f, 0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f }}, // 9
		{{ -0.5f, 0.5f, 0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f }}, // 10
		{{ -0.5f, 0.5f,-0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 0.5f }}, // 11

		// RIGHT FACE					CYAN
		{{  0.5f,-0.5f,-0.5f, 1.0f },{ 0.0f, 1.0f, 1.0f, 0.5f }}, // 12
		{{  0.5f,-0.5f, 0.5f, 1.0f },{ 0.0f, 1.0f, 1.0f, 0.5f }}, // 13
		{{  0.5f, 0.5f, 0.5f, 1.0f },{ 0.0f, 1.0f, 1.0f, 0.5f }}, // 14
		{{  0.5f, 0.5f,-0.5f, 1.0f },{ 0.0f, 1.0f, 1.0f, 0.5f }}, // 15

		// TOP FACE						BLUE
		{{ -0.5f, 0.5f,-0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 0.5f }}, // 16
		{{ -0.5f, 0.5f, 0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 0.5f }}, // 17
		{{  0.5f, 0.5f, 0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 0.5f }}, // 18
		{{  0.5f, 0.5f,-0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 0.5f }}, // 19

		// BOTTOM FACE					MAGENTA
		{{ -0.5f,-0.5f,-0.5f, 1.0f },{ 1.0f, 0.0f, 1.0f, 0.5f }}, // 20
		{{ -0.5f,-0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 1.0f, 0.5f }}, // 21
		{{  0.5f,-0.5f, 0.5f, 1.0f },{ 1.0f, 0.0f, 1.0f, 0.5f }}, // 22
		{{  0.5f,-0.5f,-0.5f, 1.0f },{ 1.0f, 0.0f, 1.0f, 0.5f }}, // 23

	};

	// Step 2
	// From the above verts, we need to construct triangles that opengl can then use to render.
	// Winding order is important, each triangle needs to be described in a clockwise order
	// this defines the facing direction for the triangle.
	// By default OpenGL will cull pixels that are "facing awway" from the camera glCullMode(GL_BACK) is the default settings.
	// Culling can be changed to GL_FRONT or GL_FRONT_AND_BACK, or enabled/disabled via glEnable(GL_CULL_FACE) / glDisable(GL_CULL_FACE)

	unsigned char indices[] =
	{
		0,1,2,		0,2,3,		// front face
		6,5,4,		7,6,4,		// back face
		8,9,10,		8,10,11,	// left face
		14,13,12,	15,14,12,	// right face
		16,17,18,	16,18,19,	// top face
		22,21,20,	23,22,20	// bottom face
	};

	// Step 3:
	// It's always a good idea to keep track of how many verts and indices we have
	// When drawing the glDrawElements method requires the number of indices
	// sizeof(verts) returns the entire size in bytes of the array
	// sizeof(Vertex) returns the size in bytes of a single vertex
	// we can calculate the number of verts or indices by dividing.
	m_vertCount = sizeof(verts) / sizeof(Vertex);
	m_indicesCount = sizeof(indices) / sizeof(unsigned char);

	// Step 4:
	// Generate the VAO and bind it.
	// Our VBO (Vertex Buffer Object) and IBO (Index Buffer Object) will be "grouped" with this VAO
	// other settings will also be grouped with the VAO. This is used so we can reduce draw calls in the render method
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Step 5:
	// Create our VBO and IBO.
	// Then tell OpenGL what type of buffer they are used for
	// VBO a buffer in graphics memory to contain our vertices
	// IBO a buffer in graphics memory to contain our indices
	// Then fill the buffers with our generated data.
	// this is taking our verts and indices from ram and sending them to the graphics card
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Step 6:
	// Vertices can have any shape, for us, each vertex has a position and a colour.
	// the "shape" of our vertex needs to be described to OpenGL
	// This is so the vertices can be sent to our shader and be mapped to the correct variables locations.
	Vertex::SetupVertexAttribPointers();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Application3D::DestroyGeometry()
{
	// When we're done, destroy the geometry
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteVertexArrays(1, &m_vao);
}

void Application3D::LoadShader()
{
	static const char* vertex_shader =
		"#version 400\n \
		in vec4 vPosition;\n \
		in vec4 vColor;\n \
		out vec4 fColor;\n \
		uniform mat4 projectionView;\n \
		void main ()\n \
		{\n \
		fColor = vColor;\n \
		gl_Position = vPosition;\n \
		}";

	static const char* fragment_shader =
		"#version 400\n \
		in vec4 fColor;\n \
		out vec4 frag_color;\n \
		void main ()\n \
		{\n \
		frag_color = fColor;\n \
		}";

	//  Step 1:
	// Load the vertex shader, provide it with the source code and compile it.
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	//  Step 2:
	// Load the fragment shader, provide it with the source code and compile it.
	GLuint fs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	// Step 3:
	// Create the shader program
	m_shader = glCreateProgram();

	// Step 4:
	// Attach the vertex and fragment shaders to the shader program
	glAttachShader(m_shader, vs);
	glAttachShader(m_shader, fs);

	// Step 5:
	// Describe the location of the shader elements
	// NOTE: the method: SetupVertexAttribPointers
	// these locations are used when describing the verts
	glBindAttribLocation(m_shader, 0, "vPosition");
	glBindAttribLocation(m_shader, 1, "vColor");

	glLinkProgram(m_shader);

	m_projectionViewLoc = glGetUniformLocation(m_shader, "projectionView");

	// Step 6:
	// destroy the vertex and fragment shader, we are finished with them
	// as they have been combined into the shaderProgram
	glDeleteShader(vs);
	glDeleteShader(fs);

}

void Application3D::UnloadShader()
{
	glDeleteProgram(m_shader);
}

void Vertex::SetupVertexAttribPointers()
{
	// enable vertex position element
	// notice when we loaded the shader, we described the "position" element to be location 0.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					// attribute 0 (position)
		4,					// size - how many floats make up the position (x, y, z, w)
		GL_FLOAT,			// type - our x, y, z, w are float values
		GL_FALSE,			// normalised? - not used
		sizeof(Vertex),		// stride - size of an entire vertex
		(void*)0			// offset - bytes from the beginning of the vertex
	);

	// enable vertex colour element
	// notice when we loaded the shader, we described the "colour" element to be location 1.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,					// attribute 1 (colour)
		4,					// size - how many floats make up the colour (r, g, b, a)
		GL_FLOAT,			// type - our r, g, b, a are float values
		GL_FALSE,			// normalised? - not used
		sizeof(Vertex),		// stride - size of an entire vertex
		(void*)(sizeof(float) * 4)	// offset - bytes from the beginning of the vertex
	);
}
