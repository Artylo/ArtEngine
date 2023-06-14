#pragma once
class VertexBuffer
{
	public:
		
	private:

	protected:
		unsigned int VBO_ID;
		
	public:
		VertexBuffer();
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
	private:
};

