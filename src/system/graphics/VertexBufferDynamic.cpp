#include "VertexBufferDynamic.h"
#include "Renderer.h"

//Default Constructor has a 1000 vertex limit. Roughly 36kb of data. You will need to call glBufferData with your vertex data to set the size, otherwise it will glitch.
VertexBufferDynamic::VertexBufferDynamic()
{
	GLCALL(glGenBuffers(1, &VBO_ID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO_ID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * default_max_vertex_count, nullptr, GL_DYNAMIC_DRAW));
}

//Generates a VBO with the size and data requirements you provide.
VertexBufferDynamic::VertexBufferDynamic(const void* data, unsigned int size)
{
	GLCALL(glGenBuffers(1, &VBO_ID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO_ID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

VertexBufferDynamic::~VertexBufferDynamic()
{
	glDeleteBuffers(1, &VBO_ID);
}
