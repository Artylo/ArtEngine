#include "VertexBufferDynamic.h"
#include "Renderer.h"

//Accepts an array of vertices and sizeof(vertices).
VertexBufferDynamic::VertexBufferDynamic()
{
	GLCALL(glGenBuffers(1, &VBO_ID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO_ID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_count, nullptr, GL_DYNAMIC_DRAW));
}

