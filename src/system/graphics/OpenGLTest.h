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
		-0.5f, -0.5f,	0.0f, 0.0f,		//Bottom Left
		 0.5f,  0.5f,	1.0f, 1.0f,		//Top Right
		 0.5f, -0.5f,	1.0f, 0.0f,		//Bottom Right
		-0.5f,  0.5f,	0.0f, 1.0f		//Top Left
	};
	unsigned int indices[6] = {
		1, 2, 0,
		0, 3, 1
	};

	//Projection Matrix
	glm::mat4 projection_matrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f);
public:

	//OpenGL variables
	unsigned int ProgramID = 0;
	unsigned int VAO = 0; // Vertex Array Object
	//unsigned int VBO = 0;
	//unsigned int IBO = 0;
	int gVertexPos2DLocation = -1;
	Renderer renderer;
	Shader* shader = nullptr;
	VertexArray* vertex_array = nullptr;
	VertexBufferLayout* vertex_buffer_layout = nullptr;
	VertexBuffer* vertex_buffer = nullptr;
	IndexBuffer* index_buffer = nullptr;
	//VertexBuffer* vertex_buffer;
	//IndexBuffer* index_buffer;

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

