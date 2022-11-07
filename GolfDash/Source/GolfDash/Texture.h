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
	private:
		uint32 m_TextureID = 0;
	};

}