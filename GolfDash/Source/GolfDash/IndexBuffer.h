#pragma once

namespace gd {

	class IndexBuffer : public ReferenceCounted
	{
	public:
		IndexBuffer(const void* data, size_t size);
		virtual ~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32 GetCount() const { return static_cast<uint32>(m_Size / sizeof(uint32)); }
	private:
		uint32 m_BufferID = 0;
		size_t m_Size = 0;
	};

}