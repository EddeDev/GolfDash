#include "GolfDashPCH.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

namespace gd {

	Texture::Texture(const std::string& filepath)
	{
		stbi_set_flip_vertically_on_load(true);
		
		int32 width, height, channels;
		uint8* data = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_default);
		if (!data)
		{
			std::cerr << "Could not load texture '" << filepath << "'" << std::endl;
			std::cerr << stbi_failure_reason() << std::endl;
			return;
		}

		m_Width = width;
		m_Height = height;

		uint32 format = 0;
		uint32 internalFormat = 0;
		if (channels == STBI_rgb)
		{
			format = GL_RGB;
			internalFormat = GL_RGB8;
		}
		else
		{
			format = GL_RGBA;
			internalFormat = GL_RGBA8;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, internalFormat, width, height);

		glTextureSubImage2D(m_TextureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, static_cast<const void*>(data));
		// glGenerateTextureMipmap(m_TextureID);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		
		stbi_image_free(data);
	}

	Texture::Texture(uint32 width, uint32 height, const void* data)
		: m_Width(width), m_Height(height)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glTextureStorage2D(m_TextureID, 1, GL_RGBA8, width, height);

		glTextureSubImage2D(m_TextureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		// glGenerateTextureMipmap(m_TextureID);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void Texture::Bind(uint32 slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}

	void Texture::Unbind(uint32 slot) const
	{
		glBindTextureUnit(slot, 0);
	}

	Ref<Texture> Texture::Create(const std::string& filepath)
	{
		if (s_TextureMap.find(filepath) != s_TextureMap.end())
			return s_TextureMap.at(filepath);

		Ref<Texture> texture = Ref<Texture>::Create(filepath);
		s_TextureMap[filepath] = texture;
		return texture;
	}

	std::unordered_map<std::string, Ref<Texture>> Texture::s_TextureMap;

}