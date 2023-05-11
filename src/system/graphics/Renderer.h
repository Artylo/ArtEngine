#pragma once

#include <GL/glew.h>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define GLASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	GLASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	
private:
public:
	void Clear() const;
	void Draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader) const;
private:
};

