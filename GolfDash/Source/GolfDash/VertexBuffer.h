#pragma once

namespace gd {

	enum class VertexBufferUsage
	{
		None = 0,
		Static,
		Dynamic
	};

	class VertexBuffer : public ReferenceCounted
	{
	public:
		VertexBuffer(const void* data, size_t size, VertexBufferUsage usage = VertexBufferUsage::Static);
		virtual ~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, size_t size) const;
	private:
		uint32 m_BufferID = 0;
	};

}