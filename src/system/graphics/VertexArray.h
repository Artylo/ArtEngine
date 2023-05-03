#pragma once

#include "../globals.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int RendererID;
public:
	VertexArray();
	~VertexArray();
	void Bind() const;
	void Unbind() const;

	void AddBuffer(const std::shared_ptr<VertexBuffer>& vb, const std::shared_ptr<VertexBufferLayout>& layout);
};

