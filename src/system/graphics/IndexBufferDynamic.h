#pragma once

class IndexBufferDynamic
{
private:
	unsigned int IBOID;
	unsigned int count;
public:
	//IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBufferDynamic(const void* data, unsigned int count);
	~IndexBufferDynamic();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return count; }
};

