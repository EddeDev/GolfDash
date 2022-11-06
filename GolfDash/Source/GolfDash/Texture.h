#pragma once

namespace gd {

	class Texture : public ReferenceCounted
	{
	public:
		Texture(const std::string& filepath);
		virtual ~Texture();

		void Bind(uint32 slot = 0) const;
		void Unbind(uint32 slot = 0) const;
	private:
		uint32 m_TextureID = 0;
	};

}