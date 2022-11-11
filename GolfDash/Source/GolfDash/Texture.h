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

		static Ref<Texture> Create(const std::string& filepath);
	private:
		uint32 m_TextureID = 0;
		uint32 m_Width = 0;
		uint32 m_Height = 0;
	
		static std::unordered_map<std::string, Ref<Texture>> s_TextureMap;
	};

}