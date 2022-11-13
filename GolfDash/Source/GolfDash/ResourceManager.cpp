#include "GolfDashPCH.h"
#include "ResourceManager.h"

#include "GolfDash.h"

#include <filesystem>

namespace gd {

	namespace Utils {

		static bool EndsWith(const std::string& value, const std::string& ending)
		{
			if (ending.size() > value.size())
				return false;

			return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
		}

	}

	static constexpr uint64 s_HeaderIdentifier = 0x37EB7D773575C;

	struct AssetPackHeader
	{
		uint64 HeaderIdentifier;
		uint32 NumShaders;
		uint32 NumTextures;
		// data
	};

	struct ShaderData
	{
		uint64 Size;
		uint32 BinaryFormat;

		uint32 NameSize;
		// data
	};

	struct TextureData
	{
		int32 Width;
		int32 Height;
		int32 Channels;
		uint32 NameSize;
		// data
	};

	void ResourceManager::Init()
	{
		std::unordered_map<std::string, ShaderInfo> shaderInfos;
		std::unordered_map<std::string, TextureInfo> textureInfos;

#ifdef GD_DIST
		std::ifstream in("Assets.gd", std::ios::binary);

		AssetPackHeader header;
		in.read((char*)&header, sizeof(AssetPackHeader));
		if (header.HeaderIdentifier != s_HeaderIdentifier)
		{
			std::cerr << "Invalid file!" << std::endl;

			GolfDash::Get().Close();
			return;
		}

		std::vector<ShaderData> shaderDataHeaders(header.NumShaders);
		std::vector<std::string> shaderNames(header.NumShaders);
		std::vector<std::vector<uint8>> shaderBinaries(header.NumShaders);

		for (uint32 i = 0; i < header.NumShaders; i++)
		{
			auto& data = shaderDataHeaders[i];
			in.read((char*)&data, sizeof(ShaderData));

			auto& name = shaderNames[i];
			name.resize(data.NameSize);
			in.read(name.data(), name.size());

			auto& binaryData = shaderBinaries[i];
			binaryData.resize(data.Size);
			in.read((char*)binaryData.data(), binaryData.size());
		}

		std::vector<TextureData> textureDataHeaders(header.NumTextures);
		std::vector<std::string> textureNames(header.NumTextures);
		std::vector<std::vector<uint8>> textureDataStorage(header.NumTextures);

		for (uint32 i = 0; i < header.NumTextures; i++)
		{
			auto& data = textureDataHeaders[i];
			in.read((char*)&data, sizeof(TextureData));

			auto& name = textureNames[i];
			name.resize(data.NameSize);
			in.read(name.data(), name.size());

			auto& binaryData = textureDataStorage[i];
			size_t dataSize = data.Width * data.Height * data.Channels;
			binaryData.resize(dataSize);
			in.read((char*)binaryData.data(), binaryData.size());
		}

		in.close();

		for (uint32 i = 0; i < header.NumShaders; i++)
		{
			std::string shaderName = shaderNames[i];

			ShaderInfo info;
			info.Binary = shaderBinaries[i];
			info.BinaryFormat = shaderDataHeaders[i].BinaryFormat;

			shaderInfos[shaderName] = info;
		}

		for (uint32 i = 0; i < header.NumTextures; i++)
		{
			std::string textureName = textureNames[i];

			TextureInfo info;
			info.Width = textureDataHeaders[i].Width;
			info.Height = textureDataHeaders[i].Height;
			info.Channels = textureDataHeaders[i].Channels;
			info.Data = textureDataStorage[i].data();

			textureInfos[textureName] = info;
		}
		
#else
		for (auto& dirEntry : std::filesystem::recursive_directory_iterator("Assets"))
		{
			const auto& path = dirEntry.path();

			if (path.extension() == ".glsl")
			{
				std::string filename = path.filename().stem().string();

				std::string shaderName = filename.substr(0, filename.find_last_of('_'));

				if (Utils::EndsWith(filename, "_Vertex"))
				{
					if (shaderInfos.find(shaderName) != shaderInfos.end())
					{
						shaderInfos.at(shaderName).VertexShaderPath = path.string();
					}
					else
					{
						ShaderInfo info;
						info.VertexShaderPath = path.string();

						shaderInfos[shaderName] = info;
					}
				}
				else if (Utils::EndsWith(filename, "_Fragment"))
				{
					if (shaderInfos.find(shaderName) != shaderInfos.end())
					{
						shaderInfos.at(shaderName).FragmentShaderPath = path.string();
					}
					else
					{
						ShaderInfo info;
						info.FragmentShaderPath = path.string();

						shaderInfos[shaderName] = info;
					}
				}
			}
			else if (path.extension() == ".psd")
			{
				std::string textureName = path.filename().stem().string();

				TextureInfo info;
				info.Filepath = path.string();

				textureInfos[textureName] = info;
			}
		}
#endif

		for (auto& [shaderName, shaderInfo] : shaderInfos)
			s_Shaders[shaderName] = Ref<Shader>::Create(shaderInfo);

		for (auto& [textureName, textureInfo] : textureInfos)
			s_Textures[textureName] = Ref<Texture>::Create(textureInfo);
	}

	void ResourceManager::Shutdown()
	{
#ifndef GD_DIST
		std::string filename("Assets.gd");
		std::ofstream out(filename.c_str(), std::ios::binary);

		AssetPackHeader header;
		header.HeaderIdentifier = s_HeaderIdentifier;
		header.NumShaders = static_cast<uint32>(s_Shaders.size());
		header.NumTextures = static_cast<uint32>(s_Textures.size());

		out.write((char*)&header, sizeof(AssetPackHeader));

		for (auto& [shaderName, shader] : s_Shaders)
		{
			auto& info = shader->GetInfo();

			ShaderData data;
			data.Size = info.Binary.size();
			data.BinaryFormat = info.BinaryFormat;
			data.NameSize = shaderName.size();

			out.write((char*)&data, sizeof(ShaderData));
			out.write(shaderName.data(), shaderName.size());
			out.write((char*)info.Binary.data(), info.Binary.size());
		}

		for (auto& [textureName, texture] : s_Textures)
		{
			auto& info = texture->GetInfo();

			TextureData data;
			data.Width = info.Width;
			data.Height = info.Height;
			data.Channels = info.Channels;
			data.NameSize = textureName.size();

			if (!info.Data)
			{
				std::cout << "Texture is null!" << std::endl;
				continue;
			}

			size_t dataSize = data.Width * data.Height * data.Channels;

			out.write((char*)&data, sizeof(TextureData));
			out.write(textureName.data(), textureName.size());
			out.write((char*)info.Data, dataSize);
		}

		out.close();
#endif
	}

	Ref<Shader> ResourceManager::GetShader(const std::string& name)
	{
		if (s_Shaders.find(name) != s_Shaders.end())
			return s_Shaders.at(name);

		return nullptr;
	}

	Ref<Texture> ResourceManager::GetTexture(const std::string& name)
	{
		if (s_Textures.find(name) != s_Textures.end())
			return s_Textures.at(name);

		return nullptr;
	}

}