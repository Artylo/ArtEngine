#include "OpenGLTest.h"

#include <stdio.h>
#include <string.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

OpenGLTest::OpenGLTest(GameManager* GM) : Test(GM)
{

}

OpenGLTest::~OpenGLTest()
{
	delete shader;
	delete vertex_buffer;
	delete index_buffer;
	delete vertex_array;
	delete vertex_buffer_layout;
	delete texture;
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

	//Init Texture
	texture = new Texture("img/player.png");
	texture->Bind(0);
}

void OpenGLTest::update()
{
	shader->Bind();
	texture->Bind();

		shader->SetUniform1f("u_Time", ((float)SDL_GetTicks())); // Current Time in Ticks
		shader->SetUniform1i("u_Texture", 0); // Slot of Texture

		int winW, winH;
		SDL_GetWindowSize(GM->window, &winW, &winH);

		projection_matrix = glm::ortho(0.0f, (float)winW/2, (float)winH/2, 0.0f, -1.0f, 1.0f);

		transformation_matrix = scale_matrix * identity_matrix;
		transformation_matrix = glm::rotate(transformation_matrix, glm::radians(camera_rotation), glm::vec3(0.0, 0.0, 1.0));
		model_view_projection = projection_matrix * view_matrix * model_matrix;
		model_view_projection = transformation_matrix * model_view_projection;
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