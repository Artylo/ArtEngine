#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : count(count)
{
	GLASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCALL(glGenBuffers(1, &IBOID));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &IBOID);
}

void IndexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID));
}

void IndexBuffer::Unbind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
