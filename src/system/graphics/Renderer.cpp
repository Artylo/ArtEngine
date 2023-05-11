#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	//Clear Colour Buffer
	glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader) const
{
	shader.Bind();
	vertex_array.Bind();
	index_buffer.Bind();

	GLCALL(glDrawElements(GL_TRIANGLES, index_buffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
