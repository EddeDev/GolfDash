#pragma once

#include "PrimitiveTypes.h"

#include <vector>
#include <iostream>

namespace gd {

	enum class ShaderDataType
	{
		None = 0,
		Float, Vec2, Vec3, Vec4,
		Mat3, Mat4,
		Int, IVec2, IVec3, IVec4,
		Bool
	};

	static uint32 ShaderDataTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return 1;
		case ShaderDataType::Vec2:  return 2;
		case ShaderDataType::Vec3:  return 3;
		case ShaderDataType::Vec4:  return 4;
		case ShaderDataType::Mat3:  return 3 * 3;
		case ShaderDataType::Mat4:  return 4 * 4;
		case ShaderDataType::Int:   return 1;
		case ShaderDataType::IVec2: return 2;
		case ShaderDataType::IVec3: return 3;
		case ShaderDataType::IVec4: return 4;
		case ShaderDataType::Bool:  return 1;
		}
		std::cerr << "Unknown ShaderDataType" << std::endl;
		return 0;
	}

	static uint32 ShaderDataTypeSize(ShaderDataType type)
	{
		const uint32 componentCount = ShaderDataTypeComponentCount(type);
		switch (type)
		{
		case ShaderDataType::Float: return componentCount * sizeof(float);
		case ShaderDataType::Vec2:  return componentCount * sizeof(float);
		case ShaderDataType::Vec3:  return componentCount * sizeof(float);
		case ShaderDataType::Vec4:  return componentCount * sizeof(float);
		case ShaderDataType::Mat3:  return componentCount * sizeof(float);
		case ShaderDataType::Mat4:  return componentCount * sizeof(float);
		case ShaderDataType::Int:   return componentCount * sizeof(int32);
		case ShaderDataType::IVec2: return componentCount * sizeof(int32);
		case ShaderDataType::IVec3: return componentCount * sizeof(int32);
		case ShaderDataType::IVec4: return componentCount * sizeof(int32);
		case ShaderDataType::Bool:  return componentCount * sizeof(bool);
		}
		std::cerr << "Unknown ShaderDataType" << std::endl;
		return 0;
	}

	struct VertexInputAttribute
	{
		std::string Name;
		ShaderDataType Type;
		uint32 Size;
		uint32 ComponentCount;
		uint32 Offset;
		bool Normalized;

		VertexInputAttribute() = default;
		VertexInputAttribute(const std::string& name, ShaderDataType type, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), ComponentCount(ShaderDataTypeComponentCount(type)), Offset(0), Normalized(normalized)
		{
		}
	};

	class VertexInputLayout
	{
	public:
		VertexInputLayout() {}
		VertexInputLayout(const std::initializer_list<VertexInputAttribute>& attributes)
			: m_Attributes(attributes)
		{
			uint32_t offset = 0;
			for (auto& attribute : m_Attributes)
			{
				attribute.Offset = offset;
				offset += attribute.Size;
				m_Stride += attribute.Size;
			}
		}

		uint32 GetStride() const { return m_Stride; };
		uint32 GetAttributeCount() const { return (uint32)m_Attributes.size(); };
		const std::vector<VertexInputAttribute>& GetAttributes() const { return m_Attributes; }

		std::vector<VertexInputAttribute>::iterator begin() { return m_Attributes.begin(); }
		std::vector<VertexInputAttribute>::iterator end() { return m_Attributes.end(); }
		std::vector<VertexInputAttribute>::const_iterator begin() const { return m_Attributes.begin(); }
		std::vector<VertexInputAttribute>::const_iterator end() const { return m_Attributes.end(); }
	private:
		std::vector<VertexInputAttribute> m_Attributes;
		uint32 m_Stride = 0;
	};

}