//STD includes
#include <fstream>
#include <sstream>

//My includes
#include "../globals.h"
#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath) : shaderFilepath(filepath) , ProgramID(0)
{
	src = ParseShader(filepath);
	ProgramID = CreateShader(src.VertexSource, src.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(ProgramID);
}

void Shader::Bind() const
{
	GLCALL(glUseProgram(ProgramID));
}

void Shader::Unbind() const
{
	GLCALL(glUseProgram(0));
}

//Parses GLSL from the filepath and returns a vertex and fragment shader.
Shader::ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};
	ShaderType type = ShaderType::NONE;

	std::stringstream ss[2];
	std::string line;
	while (std::getline(stream, line))
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

//Compiles an individual vertex/fragment shader source.
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

//Links freshly compiled shaders to the program.
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

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
	GLCALL(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCALL(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
	GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (UniformLocationCache.find(name) != UniformLocationCache.end()) return UniformLocationCache[name]; // Is this already cached?

	GLCALL(unsigned int location = glGetUniformLocation(ProgramID, name.c_str()));

	if (location == -1)
	{
		std::cout << "Could not find uniform location for - " << name << " in Shader." << std::endl;
	}

	UniformLocationCache[name] = location; // Cache the location.
	return location;
}
