#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	//Vertex Array Object
	GLCALL(glGenVertexArrays(1, &RendererID));
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &RendererID);
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(RendererID));
}

void VertexArray::Unbind() const
{
	GLCALL(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const std::shared_ptr<VertexBuffer>& vb, const std::shared_ptr<VertexBufferLayout>& layout)
{
	
	Bind(); // Bind VAO
	vb.get()->Bind(); // Bind VBO
	const auto& elements = layout.get()->GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get()->GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);	
	}
}