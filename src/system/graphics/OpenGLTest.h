#pragma once
#include "../../tests/Test.h"

#include "../globals.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "../GameManager.h";

class OpenGLTest : public test::Test
{
private:
	float vertices[16] = {
		//position		//texture coords
		 0.0f, 0.0f,	0.0f, 0.0f,		//Top Left
		 32.0f, 0.0f,	1.0f, 0.0f,		//Top Right
		 0.0f, 32.0f,	0.0f, 1.0f,		//Bottom Left
		 32.0f, 32.0f,	1.0f, 1.0f		//Bottom Right
		
	};
	unsigned int indices[6] = {
		0, 1, 2,
		2, 1, 3
	};

	//Projection Matrix
	glm::mat4 projection_matrix = glm::ortho(0.0f, (float)w_width/2, (float)w_height/2, 0.0f, -1.0f, 1.0f); // Coordinate System

public:
	GameManager* GM;

	float camera_scale = 1.0f;
	float camera_rotation = 0.0f;

	glm::mat4 identity_matrix = glm::mat4(1.0f);
	glm::mat4 scale_matrix = glm::scale(identity_matrix, glm::vec3(camera_scale, camera_scale, camera_scale));
	//glm::mat4 rotation_matrix = glm::rotate(identity_matrix, glm::radians(camera_rotation),glm::vec3(0.0,0.0,1.0));
	glm::mat4 transformation_matrix = scale_matrix * identity_matrix;
	

	glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0)); // Camera - moved 100 to the right
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0)); // Translate all vertecies by an amount.
	glm::mat4 model_view_projection = projection_matrix * view_matrix * model_matrix;
public:

	//OpenGL variables
	unsigned int ProgramID = 0;
	int gVertexPos2DLocation = -1;
	Renderer renderer;
	Shader* shader = nullptr;
	VertexArray* vertex_array = nullptr;
	VertexBufferLayout* vertex_buffer_layout = nullptr;
	VertexBuffer* vertex_buffer = nullptr;
	IndexBuffer* index_buffer = nullptr;
	Texture* texture;
	

	OpenGLTest(GameManager* GM);
	~OpenGLTest();

private:
	

public:
	void CreateVBO();
	void CreateIBO();
	void init();
	void update();
	void draw();
	void printShaderError();


};

