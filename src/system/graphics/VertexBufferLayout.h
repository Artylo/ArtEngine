#pragma once

#include <vector>
#include "Renderer.h"

//OpenGL includes
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>


struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:	return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:	return sizeof(GLubyte);
		}
		GLASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> Elements;
	unsigned int stride = 0;
public:
	VertexBufferLayout();
	~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count)
	{
		assert(false); 

		//If you are here, then whatever you're pushing needs you to create a templated overload for Push().
	}

	template<>
	void Push<float>(unsigned int count)
	{
		Elements.push_back({GL_FLOAT, count, GL_FALSE});
		stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		Elements.push_back({GL_UNSIGNED_INT,count,GL_FALSE});
		stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		Elements.push_back({GL_UNSIGNED_BYTE,count,GL_TRUE});
		stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	template<>
	void Push<glm::vec2>(unsigned int count)
	{
		Elements.push_back({ GL_FLOAT,count * 2,GL_TRUE });
		stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * (count * 2);
	}

	template<>
	void Push<glm::vec3>(unsigned int count)
	{
		Elements.push_back({ GL_FLOAT,count * 3,GL_TRUE });
		stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * (count * 3);
	}

	template<>
	void Push<glm::vec4>(unsigned int count)
	{
		Elements.push_back({ GL_FLOAT,count * 4,GL_TRUE });
		stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * (count * 4);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return Elements; }
	inline unsigned int GetStride() const { return stride; }

};

