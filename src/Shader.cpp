#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader()
{

}

Shader::~Shader()
{
	glDeleteProgram(ProgramID);
}

Shader::ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT =1
	};
	ShaderType type = ShaderType::NONE;

	std::stringstream ss[2];
	std::string line;
	while (std::getline(stream, line) )
	{
		if (line.find("#shader") != std::string::npos) // Find Header
		{
			if (line.find("vertex") != std::string::npos) // Is it a vertex shader?
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)  // Is it a fragment shader?
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int  shaderCompileStatus;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &shaderCompileStatus);
	if (shaderCompileStatus != GL_TRUE)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		SDL_Log("ERROR: Unable to compile %s shader %s!\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), infoLog);
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int p = glCreateProgram();
	unsigned int v = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int f = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(p, v);
	glAttachShader(p, f);

	glLinkProgram(p);
	glValidateProgram(p);

	glDeleteShader(v);
	glDeleteShader(f);

	return p;
}

void Shader::CreateVBO()
{
	float vertices[6] = {
	-0.5f, -0.5f,
	0.0f, 0.5f,
	0.5f, -0.5f
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
}

void Shader::init()
{
	src = ParseShader("shader/base.shader");
	ProgramID = CreateShader(src.VertexSource, src.FragmentSource);
}

void Shader::update()
{

}

void Shader::draw()
{
	//Render
	if (true) // If this thing is supposed to be rendered. Is it on screen, etc.?
	{
		glUseProgram(ProgramID);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glUseProgram(NULL);
	}
}