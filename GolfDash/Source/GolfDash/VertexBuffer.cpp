#include "GolfDashPCH.h"
#include "VertexBuffer.h"

#include <glad/glad.h>

namespace gd {

	namespace Utils {

		static uint32 OpenGLVertexBufferUsage(VertexBufferUsage usage)
		{
			switch (usage)
			{
			case VertexBufferUsage::Static: return GL_STATIC_DRAW;
			case VertexBufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
			}
			std::cout << "Unknown VertexBufferUsage" << std::endl;
			return 0;
		}

	}

	VertexBuffer::VertexBuffer(const void* data, size_t size, VertexBufferUsage usage)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, size, data, Utils::OpenGLVertexBufferUsage(usage));
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, size_t size) const
	{
		glNamedBufferSubData(m_BufferID, 0, size, data);
	}

}