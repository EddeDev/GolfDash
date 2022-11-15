#include "GolfDashPCH.h"
#include "Shader.h"

#include <glad/glad.h>

namespace gd {
	
	static uint32 CreateShader(uint32 type, const std::string& source)
	{
		uint32 shaderID = glCreateShader(type);

		const char* sourceCStr = source.c_str();
		glShaderSource(shaderID, 1, &sourceCStr, 0);
		glCompileShader(shaderID);

		int32 isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			int32 maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, infoLog.data());
			std::cerr << infoLog.data() << std::endl;

			glDeleteShader(shaderID);
		}

		return shaderID;
	}

	Shader::Shader(const ShaderInfo& info)
		: m_Info(info)
	{
		uint32 vertexShaderID = CreateShader(GL_VERTEX_SHADER, m_Info.VertexShaderSource);
		uint32 fragmentShaderID = CreateShader(GL_FRAGMENT_SHADER, m_Info.FragmentShaderSource);

		m_ProgramID = glCreateProgram();
		glAttachShader(m_ProgramID, vertexShaderID);
		glAttachShader(m_ProgramID, fragmentShaderID);

		glLinkProgram(m_ProgramID);

		int32 isLinked = 0;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			int32 maxLength = 0;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(m_ProgramID, maxLength, &maxLength, infoLog.data());
			std::cerr << infoLog.data() << std::endl;

			glDeleteProgram(m_ProgramID);

			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
			return;
		}

		glDetachShader(m_ProgramID, vertexShaderID);
		glDetachShader(m_ProgramID, fragmentShaderID);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ProgramID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniform(const std::string& uniformName, uint32 value) const
	{
		uint32 location = glGetUniformLocation(m_ProgramID, uniformName.c_str());
		if (location == -1)
			std::cerr << "Could not find uniform in shader!" << std::endl;

		glUniform1ui(location, value);
	}

	void Shader::SetUniform(const std::string& uniformName, float value) const
	{
		uint32 location = glGetUniformLocation(m_ProgramID, uniformName.c_str());
		if (location == -1)
			std::cerr << "Could not find uniform in shader!" << std::endl;

		glUniform1f(location, value);
	}

	void Shader::SetUniform(const std::string& uniformName, const glm::mat4& value) const
	{
		uint32 location = glGetUniformLocation(m_ProgramID, uniformName.c_str());
		if (location == -1)
			std::cerr << "Could not find uniform in shader!" << std::endl;

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

}