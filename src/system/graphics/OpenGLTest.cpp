#include "OpenGLTest.h"

#include <stdio.h>
#include <string.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

OpenGLTest::OpenGLTest()
{

}

OpenGLTest::~OpenGLTest()
{
	delete shader;
	delete vertex_buffer;
	delete index_buffer;
	delete vertex_array;
	delete vertex_buffer_layout;
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &TextureID);
}

//Flips a SDL_Surface vertically, so that its pixel data is in the correct format for being converted into an OpenGL texture.
void OpenGLTest::FlipSurface(SDL_Surface* surface)
{
	SDL_LockSurface(surface);
		int pitch = surface->pitch;
		char* temp = new char[pitch]; // Intermediate buffer.
		char* pixels = (char*)surface->pixels;

		for (int i = 0; i < surface->h / 2; ++i)
		{
			//Get pointers to rows that need to be swapped.
			char* row1 = pixels + (static_cast<int64_t>(i) * pitch);
			char* row2 = pixels + ((static_cast<int64_t>(surface->h) - i - 1) * pitch);

			//Swap rows.
			memcpy(temp, row1, pitch);
			memcpy(row1, row2, pitch);
			memcpy(row2, temp, pitch);
		}
		delete[] temp;

	SDL_UnlockSurface(surface);
}

//Creates the vertex array and loads it as a vertex buffer object.
//@TODO: Make this changable at runtime somehow.
void OpenGLTest::CreateVBO()
{
	//vertex_array = std::make_shared<VertexArray>();
	vertex_array = new VertexArray();
	vertex_buffer_layout = new VertexBufferLayout();
	
	vertex_buffer = new VertexBuffer(vertices, sizeof(vertices));

	//vertex_buffer_layout = std::make_shared<VertexBufferLayout>();
	vertex_buffer_layout->Push<float>(2); // Position
	vertex_buffer_layout->Push<float>(2); // Texture Coordinates

	vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout); //@DEBUG: May have unforseen consequences and not in the Half-Life sense.
}

//Creates the index array and loads it as an index buffer object.
//@TODO: Make this changable at runtime somehow.
void OpenGLTest::CreateIBO()
{
	//index_buffer = std::make_shared<IndexBuffer>(indices, 6);
	index_buffer = new IndexBuffer(indices, 6);
}

//Creates the Vertex Buffer Object, the Index Buffer Object, parses the shader from the filepath and loads it as the program.
void OpenGLTest::init()
{
	CreateVBO();
	CreateIBO();

	//Blending
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//Init Shader
	shader = new Shader("shader/base.shader");
	shader->Bind();

	//Generate Texture
	surface = IMG_Load("img/player.png");
	//FlipSurface(surface);
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
	shader->Bind();

		shader->SetUniform1f("u_Time", ((float)SDL_GetTicks())); // Current Time in Ticks
		shader->SetUniform1i("u_Texture", 0); // Slot of Texture

		model_view_projection = projection_matrix * view_matrix * model_matrix;
		shader->SetUniformMat4f("u_MVP", model_view_projection);

	printShaderError();
}

//Binds the currently stored program and draws the primitive - currently a rectangle.
void OpenGLTest::draw()
{
	//Render
	if (true) // If this thing is supposed to be rendered. Is it on screen, etc.?
	{
		//renderer.Clear(); //@CLEANUP: Removed for Test Framework
		renderer.Draw(*vertex_array, *index_buffer, *shader);
	}
	printShaderError();
}

void OpenGLTest::printShaderError()
{
	while (GLenum error = glGetError() != GL_NO_ERROR) // Poll for OpenGL Errors
	{
		//GLenum error = glGetError();
		SDL_Log("OpenGL Error code: %i\n", error);
		
	}
}