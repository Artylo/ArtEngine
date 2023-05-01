#include "OpenGLTest.h"

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>


OpenGLTest::OpenGLTest()
{

}

OpenGLTest::~OpenGLTest()
{
	glDeleteProgram(ProgramID);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &TextureID);

	//@DEBUG: Figure out how to do this. Probably use smart pointers.
	//delete vertex_buffer;
	//delete index_buffer;
}

//Parses GLSL from the filepath and returns a vertex and fragment shader.
OpenGLTest::ShaderProgramSource OpenGLTest::ParseShader(const std::string& filepath)
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

void OpenGLTest::FlipSurface(SDL_Surface* surface)
{
	SDL_LockSurface(surface);
		int pitch = surface->pitch;
		char* temp = new char[pitch]; // Intermediate buffer.
		char* pixels = (char*)surface->pixels;

		for (int i = 0; i < surface->h / 2; ++i)
		{
			//Get pointers to rows that need to be swapped.
			//@CLEANUP: See how you can remove the overflow errors. Perhaps switching from char.
			char* row1 = pixels + i * pitch;
			char* row2 = pixels + (surface->h - i - 1) * pitch;

			//Swap rows.
			memcpy(temp, row1, pitch);
			memcpy(row1, row2, pitch);
			memcpy(row2, temp, pitch);
		}
		delete[] temp;

	SDL_UnlockSurface(surface);
}

//Compiles an individual vertex/fragment shader source.
unsigned int OpenGLTest::CompileShader(unsigned int type, const std::string& source)
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
unsigned int OpenGLTest::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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
void OpenGLTest::CreateVBO()
{
	//Vertex Array Object
	GLCALL(glGenVertexArrays(1, &VAO));
	GLCALL(glBindVertexArray(VAO));

	vertex_buffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	//vertex_buffer = new VertexBuffer(vertices, sizeof(vertices));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL); // Positions 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL); // Texture Coordinates.
}

//Creates the index array and loads it as an index buffer object.
//@TODO: Make this changable at runtime somehow.
void OpenGLTest::CreateIBO()
{
	index_buffer = std::make_unique<IndexBuffer>(indices, 6);
	//index_buffer = new IndexBuffer(indices, 6);
}

//Creates the Vertex Buffer Object, the Index Buffer Object, parses the shader from the filepath and loads it as the program.
void OpenGLTest::init()
{
	CreateVBO();
	CreateIBO();

	src = ParseShader("shader/base.shader");
	ProgramID = CreateShader(src.VertexSource, src.FragmentSource);

	//Generate Texture
	surface = IMG_Load("img/player.png");
	FlipSurface(surface);
	assert(surface != nullptr);
	glGenTextures(1, &TextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //Scaling Filtering?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Wrapping around?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int mode = GL_RGB;
	if (surface->format->BytesPerPixel == 4) mode = GL_RGBA; //Does Texture have alpha?

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	//@TODO: Other glTex* stuff here.

	//if(surface != nullptr) SDL_FreeSurface(surface); // At some point. Or replace with smart pointer.
}

void OpenGLTest::update()
{
	GLCALL(glUseProgram(ProgramID));

		//int location = glGetUniformLocation(ProgramID, "u_Colour");
		int location2 = glGetUniformLocation(ProgramID, "u_Time");
		int location3 = glGetUniformLocation(ProgramID, "u_Texture");

		//glUniform4f(location, r, g, b, 1.0f);
		glUniform1f(location2, ((float)SDL_GetTicks()));
		glUniform1i(location3, 0); // Slot of Texture

	GLCALL(glUseProgram(NULL));
	printShaderError();
}

//Binds the currently stored program and draws the primitive - currently a rectangle.
void OpenGLTest::draw()
{
	//Render
	if (true) // If this thing is supposed to be rendered. Is it on screen, etc.?
	{
		GLCALL(glUseProgram(ProgramID));
		vertex_buffer.get()->Bind(); //@CLEANUP: Unnecessary.
		 index_buffer.get()->Bind(); //@CLEANUP: Unnecessary.
			GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		GLCALL(glUseProgram(NULL));
		printShaderError();
	}
}

void OpenGLTest::printShaderError()
{
	while (GLenum error = glGetError() != GL_NO_ERROR) // Poll for OpenGL Errors
	{
		//GLenum error = glGetError();
		SDL_Log("OpenGL Error code: %i\n", error);
		
	}
}