#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout; //@DEBUG: Fixes cyclical dependency with VertexBufferLayout calling GLASSERT.

class VertexArray
{
private:
	unsigned int VAOID;
public:
	VertexArray();
	~VertexArray();
	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};

