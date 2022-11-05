#include "GolfDashPCH.h"
#include "Pipeline.h"

#include <glad/glad.h>

namespace gd {

	namespace Utils {

		static uint32 OpenGLPrimitiveTopology(PrimitiveTopology topology)
		{
			switch (topology)
			{
			case PrimitiveTopology::Points:        return GL_POINTS;
			case PrimitiveTopology::Lines:         return GL_LINES;
			case PrimitiveTopology::Triangles:     return GL_TRIANGLES;
			case PrimitiveTopology::LineStrip:     return GL_LINE_STRIP;
			case PrimitiveTopology::TriangleStrip: return GL_TRIANGLE_STRIP;
			}
			std::cerr << "Unknown PrimitiveTopology" << std::endl;
			return 0;
		}

		static uint32 OpenGLShaderDataType(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float: return GL_FLOAT;
			case ShaderDataType::Vec2:  return GL_FLOAT;
			case ShaderDataType::Vec3:  return GL_FLOAT;
			case ShaderDataType::Vec4:  return GL_FLOAT;
			case ShaderDataType::Mat3:  return GL_FLOAT;
			case ShaderDataType::Mat4:  return GL_FLOAT;
			case ShaderDataType::Int:   return GL_INT;
			case ShaderDataType::IVec2: return GL_INT;
			case ShaderDataType::IVec3: return GL_INT;
			case ShaderDataType::IVec4: return GL_INT;
			case ShaderDataType::Bool:  return GL_BOOL;
			}
			std::cerr << "Unknown ShaderDataType" << std::endl;
			return 0;
		}

		static uint32 OpenGLDepthComparisonFunc(DepthComparisonFunc func)
		{
			switch (func)
			{
			case DepthComparisonFunc::Never:          return GL_NEVER;
			case DepthComparisonFunc::Less:           return GL_LESS;
			case DepthComparisonFunc::Equal:          return GL_EQUAL;
			case DepthComparisonFunc::LessOrEqual:    return GL_LEQUAL;
			case DepthComparisonFunc::Greater:        return GL_GREATER;
			case DepthComparisonFunc::NotEqual:       return GL_NOTEQUAL;
			case DepthComparisonFunc::GreaterOrEqual: return GL_GEQUAL;
			case DepthComparisonFunc::Always:         return GL_ALWAYS;
			}
			std::cerr << "Unknown DepthComparisonFunc" << std::endl;
			return 0;
		}

	}

	Pipeline::Pipeline(const PipelineConfig& config)
		: m_Config(config)
	{
		if (m_VertexArrayID)
			glDeleteVertexArrays(1, &m_VertexArrayID);

		glCreateVertexArrays(1, &m_VertexArrayID);
	}

	Pipeline::~Pipeline()
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void Pipeline::Bind() const
	{
		glBindVertexArray(m_VertexArrayID);
	
		const auto& layout = m_Config.InputLayout;
		uint32 attributeIndex = 0;
		for (const auto& attribute : layout)
		{
			glEnableVertexAttribArray(attributeIndex);

			uint32_t type = Utils::OpenGLShaderDataType(attribute.Type);
			if (type == GL_INT)
				glVertexAttribIPointer(attributeIndex, attribute.ComponentCount, type, layout.GetStride(), (const void*)(intptr_t)attribute.Offset);
			else
				glVertexAttribPointer(attributeIndex, attribute.ComponentCount, type, attribute.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(intptr_t)attribute.Offset);

			attributeIndex++;
		}

#define GL_SET_PROPERTY(cap, enabled) if (enabled) glEnable(cap); else glDisable(cap);

		glPolygonMode(GL_FRONT_AND_BACK, m_Config.Wireframe ? GL_LINE : GL_FILL);
		GL_SET_PROPERTY(GL_DEPTH_CLAMP, false);
		GL_SET_PROPERTY(GL_RASTERIZER_DISCARD, false);
		glLineWidth(m_Config.LineWidth);
		GL_SET_PROPERTY(GL_LINE_SMOOTH, m_Config.AntialiasedLines);

		GL_SET_PROPERTY(GL_DEPTH_TEST, m_Config.DepthTest);
		glDepthMask(m_Config.DepthWrite);
		glDepthFunc(Utils::OpenGLDepthComparisonFunc(m_Config.DepthCompareFunc));
		glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	}

	void Pipeline::Unbind() const
	{
		glBindVertexArray(0);
	}
	
	void Pipeline::DrawIndexed(uint32 count)
	{
		glDrawElements(Utils::OpenGLPrimitiveTopology(m_Config.Topology), count, GL_UNSIGNED_INT, nullptr);
	}

}