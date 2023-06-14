#pragma once
#include "VertexBuffer.h"

class VertexBufferDynamic : public VertexBuffer
{
private:
	unsigned int vertex_count = 1000;
public:
	VertexBufferDynamic();
};
