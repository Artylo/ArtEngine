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

//Parses GLSL from the filepath and returns a vertex and fragment shader.
Shader::ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT =1
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

//Creates the vertex array and loads it as a vertex buffer object.
//@TODO: Make this changable at runtime somehow.
void Shader::CreateVBO()
{
	float vertices[] = {
		-0.5f, -0.5f,
		 0.5f,  0.5f,
		 0.5f, -0.5f,
		-0.5f,  0.5f
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
}
//Creates the index array and loads it as an index buffer object.
//@TODO: Make this changable at runtime somehow.
void Shader::CreateIBO()
{
	unsigned int indices[] = {
		1, 2, 0, 
		0, 3, 1
	};

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

//Creates the Vertex Buffer Object, the Index Buffer Object, parses the shader from the filepath and loads it as the program.
void Shader::init()
{
	CreateVBO();
	CreateIBO();

	src = ParseShader("shader/base.shader");
	ProgramID = CreateShader(src.VertexSource, src.FragmentSource);
}

void Shader::update()
{
	glUseProgram(ProgramID);

		int location = glGetUniformLocation(ProgramID, "u_Colour");

		float increment = 0.01f;

		if (r < 1.0f) r += increment;
		else
		{
			if (g < 1.0f) g += increment;
			else
			{
				if (b < 1.0f) b += increment;
				else
				{
					b -= increment;
				}
				g -= increment;
			}
			r -= increment;
		}

		glUniform4f(location, r, g, b, 1.0f);
	glUseProgram(NULL);
	printShaderError();
}

//Binds the currently stored program and draws the primitive - currently a rectangle.
void Shader::draw()
{
	//Render
	if (true) // If this thing is supposed to be rendered. Is it on screen, etc.?
	{
		glUseProgram(ProgramID);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glUseProgram(NULL);
		printShaderError();
	}
}

void Shader::printShaderError()
{
	while (GLenum error = glGetError() != GL_NO_ERROR) // Poll for OpenGL Errors
	{
		//GLenum error = glGetError();
		SDL_Log("OpenGL Error code: %i\n", error);
		
	}
}