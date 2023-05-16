#include "VertexBuffer.h"
#include "Renderer.h"

//Accepts an array of vertices and sizeof(vertices).
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCALL(glGenBuffers(1, &VBOID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBOID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &VBOID);
}

//Vertex Buffer gets bound automatically in the Vertex Array, but can be done manually.
void VertexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBOID));
}

void VertexBuffer::Unbind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
