#pragma once
#include "VertexBuffer.h"

class VertexBufferDynamic : public VertexBuffer
{
private:
	unsigned int default_max_vertex_count = 1000;
public:
	VertexBufferDynamic();
	VertexBufferDynamic(const void* data, unsigned int size);

	~VertexBufferDynamic();
};
