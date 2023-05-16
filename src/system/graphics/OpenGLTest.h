#pragma once

#include "../globals.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

class OpenGLTest
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
	glm::mat4 projection_matrix = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f); // Coordinate System

public:
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

	//Texture Variables
	SDL_Surface* surface = nullptr;
	GLuint TextureID = 0;

	OpenGLTest();
	~OpenGLTest();

private:
	void FlipSurface(SDL_Surface* surface);

public:
	void CreateVBO();
	void CreateIBO();
	void init();
	void update();
	void draw();
	void printShaderError();


};

