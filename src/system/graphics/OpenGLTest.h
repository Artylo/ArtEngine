#pragma once

#include "../globals.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

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
public:
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	//OpenGL variables
	ShaderProgramSource src;
	unsigned int ProgramID = 0;
	unsigned int VAO = 0; // Vertex Array Object
	//unsigned int VBO = 0;
	//unsigned int IBO = 0;
	int gVertexPos2DLocation = -1;
	std::unique_ptr<VertexBuffer> vertex_buffer;
	std::unique_ptr<IndexBuffer> index_buffer;
	//VertexBuffer* vertex_buffer;
	//IndexBuffer* index_buffer;

	//Texture Variables
	SDL_Surface* surface;
	GLuint TextureID = 0;

	OpenGLTest();
	~OpenGLTest();

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	 
	void FlipSurface(SDL_Surface* surface);

public:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	void CreateVBO();
	void CreateIBO();
	void init();
	void update();
	void draw();
	void printShaderError();


};

