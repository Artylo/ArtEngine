#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	//Vertex Array Object
	GLCALL(glGenVertexArrays(1, &VAOID));
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &VAOID);
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(VAOID));
}

void VertexArray::Unbind() const
{
	GLCALL(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	
	Bind(); // Bind VAO
	vb.Bind(); // Bind VBO
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);	
	}
}