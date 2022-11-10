#pragma once

namespace gd {

	class Texture : public ReferenceCounted
	{
	public:
		Texture(const std::string& filepath);
		Texture(uint32 width, uint32 height, const void* data);
		virtual ~Texture();

		void Bind(uint32 slot = 0) const;
		void Unbind(uint32 slot = 0) const;

		uint32 GetWidth() const { return m_Width; }
		uint32 GetHeight() const { return m_Height; }
	private:
		uint32 m_TextureID = 0;
		uint32 m_Width = 0;
		uint32 m_Height = 0;
	};

}