#include "./system/globals.h"
#pragma once

class Shader
{
public:
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	//OpenGL variables
	ShaderProgramSource src;
	unsigned int ProgramID = 0;
	unsigned int VBO = 0;
	unsigned int IBO = 0;
	int gVertexPos2DLocation = -1;

	//Fragment Shader Variables
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;

	Shader();
	~Shader();

	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	void CreateVBO();
	void CreateIBO();
	void init();
	void update();
	void draw();
	void printShaderError();


};

