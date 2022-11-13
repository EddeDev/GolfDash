#pragma once

namespace gd {
	
	struct ShaderInfo
	{
		std::string VertexShaderPath;
		std::string FragmentShaderPath;

		std::vector<uint8> Binary;
		uint32 BinaryFormat = 0;
	};

	class Shader : public ReferenceCounted
	{
	public:
		Shader(const ShaderInfo& info);
		virtual ~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform(const std::string& uniformName, uint32 value) const;
		void SetUniform(const std::string& uniformName, float value) const;
		void SetUniform(const std::string& uniformName, const glm::mat4& value) const;

		ShaderInfo& GetInfo() { return m_Info; }
		const ShaderInfo& GetInfo() const { return m_Info; }
	private:
		void LoadShaderFromSource();
		void LoadShaderFromBinary();
	private:
		uint32 m_ProgramID = 0;
		ShaderInfo m_Info;
	};

}