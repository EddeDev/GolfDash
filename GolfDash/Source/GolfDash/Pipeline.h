#pragma once

#include "Shader.h"
#include "VertexInputLayout.h"

namespace gd {

	enum class PrimitiveTopology
	{
		None = 0,

		Points,
		Lines,
		Triangles,
		LineStrip,
		TriangleStrip
	};

	enum class DepthComparisonFunc
	{
		Never = 0,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		Always
	};

	struct PipelineConfig
	{
		VertexInputLayout InputLayout;
		Ref<Shader> Shader;
		PrimitiveTopology Topology = PrimitiveTopology::Triangles;
		DepthComparisonFunc DepthCompareFunc = DepthComparisonFunc::Less;

		bool DepthTest = true;
		bool DepthWrite = true;
		bool Wireframe = false;

		float LineWidth = 2.0f;
		bool AntialiasedLines = false;
	};

	class Pipeline : public ReferenceCounted
	{
	public:
		Pipeline(const PipelineConfig& config);
		virtual ~Pipeline();

		void Bind() const;
		void Unbind() const;

		void DrawIndexed(uint32 count);
	private:
		PipelineConfig m_Config;

		uint32 m_VertexArrayID = 0;
	};

}