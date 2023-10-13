#include "IndexBufferDynamic.h"
#include "Renderer.h"

IndexBufferDynamic::IndexBufferDynamic(const void* data, unsigned int count) : count(count)
{
	GLASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCALL(glGenBuffers(1, &IBOID));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
}

IndexBufferDynamic::~IndexBufferDynamic()
{
	glDeleteBuffers(1, &IBOID);
}

void IndexBufferDynamic::Bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOID));
}

void IndexBufferDynamic::Unbind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
