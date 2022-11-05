#pragma once

namespace gd {

	class Shader : public ReferenceCounted
	{
	public:
		Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		virtual ~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		uint32 m_ProgramID = 0;
	};

}