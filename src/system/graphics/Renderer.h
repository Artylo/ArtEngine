#pragma once

#include "glm/glm.hpp"
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

static const size_t MaxQuadCount   = 1000;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount  = MaxQuadCount * 6;
static const size_t MaxTextures = 32; //@TODO: Query this from the drivers at some point.

class Renderer
{
public:
	
private:
public:
	void Clear() const;
	void Draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader) const;
private:
};

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texture_coordinates;
	float texture_index;
};
