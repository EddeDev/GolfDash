#include "GolfDashPCH.h"
#include "Renderer.h"

#include "glad/glad.h"

namespace gd {

	Renderer::Renderer()
	{
		// Default
		{
			m_QuadShader = Ref<Shader>::Create("Assets/Shaders/Quad_Vertex.glsl", "Assets/Shaders/Quad_Fragment.glsl");

			PipelineConfig pipelineConfig;
			pipelineConfig.Topology = PrimitiveTopology::Triangles;
			pipelineConfig.Shader = m_QuadShader;
			pipelineConfig.InputLayout = {
				{ "a_Position", ShaderDataType::Vec3 },
				{ "a_Color", ShaderDataType::Vec4 },
				{ "a_TexCoord", ShaderDataType::Vec2 },
				{ "a_TexIndex", ShaderDataType::Float },
				{ "a_TilingFactor", ShaderDataType::Float },
			};
			m_QuadPipeline = Ref<Pipeline>::Create(pipelineConfig);

			m_QuadVertexBuffer = Ref<VertexBuffer>::Create(nullptr, s_MaxQuadVertices * sizeof(QuadVertex), VertexBufferUsage::Dynamic);
			m_QuadVertexStorage = new QuadVertex[s_MaxQuadVertices];
			memset(m_QuadVertexStorage, 0, s_MaxQuadVertices * sizeof(QuadVertex));

			// Indices
			uint32* indices = new uint32[s_MaxQuadIndices];
			uint32 offset = 0;
			for (uint32 i = 0; i < s_MaxQuadIndices; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			m_QuadIndexBuffer = Ref<IndexBuffer>::Create(indices, s_MaxQuadIndices * sizeof(uint32));
			delete[] indices;

			// Vertex positions
			m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			m_QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
			m_QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
			m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

			// Texture coords
			m_QuadTextureCoords[0] = { 0.0f, 0.0f };
			m_QuadTextureCoords[1] = { 1.0f, 0.0f };
			m_QuadTextureCoords[2] = { 1.0f, 1.0f };
			m_QuadTextureCoords[3] = { 0.0f, 1.0f };
		}

		// Line
		{
			m_LineShader = Ref<Shader>::Create("Assets/Shaders/Line_Vertex.glsl", "Assets/Shaders/Line_Fragment.glsl");

			PipelineConfig pipelineConfig;
			pipelineConfig.Topology = PrimitiveTopology::Lines;
			pipelineConfig.Shader = m_LineShader;
			pipelineConfig.InputLayout = {
				{ "a_Position", ShaderDataType::Vec3 },
				{ "a_Color", ShaderDataType::Vec4 }
			};
			m_LinePipeline = Ref<Pipeline>::Create(pipelineConfig);

			m_LineVertexBuffer = Ref<VertexBuffer>::Create(nullptr, s_MaxLineVertices * sizeof(LineVertex), VertexBufferUsage::Dynamic);
			m_LineVertexStorage = new LineVertex[s_MaxLineVertices];
			memset(m_LineVertexStorage, 0, s_MaxLineVertices * sizeof(LineVertex));

			uint32* indices = new uint32[s_MaxLines];
			for (uint32 i = 0; i < s_MaxLines; i++)
				indices[i] = i;
			m_LineIndexBuffer = Ref<IndexBuffer>::Create(indices, s_MaxLines * sizeof(uint32));
			delete[] indices;
		}

		// Composite
		{
			m_CompositeShader = Ref<Shader>::Create("Assets/Shaders/Composite_Vertex.glsl", "Assets/Shaders/Composite_Fragment.glsl");

			PipelineConfig pipelineConfig;
			pipelineConfig.Shader = m_CompositeShader;
			pipelineConfig.InputLayout = {
				{ "a_Position", ShaderDataType::Vec3 },
				{ "a_TexCoord", ShaderDataType::Vec2 }
			};
			m_CompositePipeline = Ref<Pipeline>::Create(pipelineConfig);
		}

		// White texture
		uint32 whiteTextureData = 0xffffffff;
		m_TextureSlots[0] = Ref<Texture>::Create(1, 1, &whiteTextureData);
	}

	Renderer::~Renderer()
	{
		delete[] m_QuadVertexStorage;
	}

	void Renderer::BeginFrame(const Camera& camera)
	{
		m_ViewProjectionMatrix = camera.ViewProjectionMatrix;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);

		m_QuadVertexPointer = m_QuadVertexStorage;
		m_QuadIndexCount = 0;

		m_LineVertexPointer = m_LineVertexStorage;
		m_LineIndexCount = 0;

		m_TextureSlotIndex = 1;
	}

	void Renderer::EndFrame()
	{
		if (m_QuadIndexCount)
		{
			size_t bufferSize = static_cast<size_t>((uint8*)m_QuadVertexPointer - (uint8*)m_QuadVertexStorage);
			m_QuadVertexBuffer->SetData(m_QuadVertexStorage, bufferSize);

			m_QuadVertexBuffer->Bind();
			m_QuadPipeline->Bind();
			m_QuadIndexBuffer->Bind();

			for (uint32 i = 0; i < m_TextureSlotIndex; i++)
				m_TextureSlots[i]->Bind(i);

			m_QuadShader->Bind();
			m_QuadShader->SetUniform("u_Uniforms.ViewProjectionMatrix", m_ViewProjectionMatrix);

			m_QuadPipeline->DrawIndexed(m_QuadIndexCount);
		}

		if (m_LineIndexCount)
		{
			size_t bufferSize = static_cast<size_t>((uint8*)m_LineVertexPointer - (uint8*)m_LineVertexStorage);
			m_LineVertexBuffer->SetData(m_LineVertexStorage, bufferSize);

			m_LineVertexBuffer->Bind();
			m_LinePipeline->Bind();
			m_LineIndexBuffer->Bind();

			m_LineShader->Bind();
			m_LineShader->SetUniform("u_Uniforms.ViewProjectionMatrix", m_ViewProjectionMatrix);

			m_LinePipeline->DrawIndexed(m_LineIndexCount);
		}
	}

	void Renderer::FlushQuads()
	{
		if (m_QuadIndexCount)
		{
			size_t bufferSize = static_cast<size_t>((uint8*)m_QuadVertexPointer - (uint8*)m_QuadVertexStorage);
			m_QuadVertexBuffer->SetData(m_QuadVertexStorage, bufferSize);

			m_QuadVertexBuffer->Bind();
			m_QuadPipeline->Bind();
			m_QuadIndexBuffer->Bind();

			for (uint32 i = 0; i < m_TextureSlotIndex; i++)
				m_TextureSlots[i]->Bind(i);

			m_QuadShader->Bind();
			m_QuadShader->SetUniform("u_Uniforms.ViewProjectionMatrix", m_ViewProjectionMatrix);

			m_QuadPipeline->DrawIndexed(m_QuadIndexCount);
		}

		m_QuadVertexPointer = m_QuadVertexStorage;
		m_QuadIndexCount = 0;

		m_TextureSlotIndex = 1;
	}

	void Renderer::RenderQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, Ref<Texture> texture, float tilingFactor)
	{
		if (m_QuadIndexCount >= s_MaxQuadIndices)
			FlushQuads();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

		float textureIndex = 0.0f;
		for (uint32 i = 1; i < m_TextureSlotIndex; i++)
		{
			if (m_TextureSlots[i].Get() == texture.Get())
			{
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (m_TextureSlotIndex >= s_MaxTextureSlots)
				FlushQuads();

			textureIndex = (float)m_TextureSlotIndex;
			m_TextureSlots[m_TextureSlotIndex] = texture;
			m_TextureSlotIndex++;
		}

		for (size_t i = 0; i < 4; i++)
		{
			m_QuadVertexPointer->Position = transform * m_QuadVertexPositions[i];
			m_QuadVertexPointer->Color = color;
			m_QuadVertexPointer->TexCoord = m_QuadTextureCoords[i];
			m_QuadVertexPointer->TexIndex = textureIndex;
			m_QuadVertexPointer->TilingFactor = tilingFactor;
			m_QuadVertexPointer++;
		}

		m_QuadIndexCount += 6;
	}

	void Renderer::FlushLines()
	{
		if (m_LineIndexCount)
		{
			size_t bufferSize = static_cast<size_t>((uint8*)m_LineVertexPointer - (uint8*)m_LineVertexStorage);
			m_LineVertexBuffer->SetData(m_LineVertexStorage, bufferSize);

			m_LineVertexBuffer->Bind();
			m_LinePipeline->Bind();
			m_LineIndexBuffer->Bind();

			m_LineShader->Bind();
			m_LineShader->SetUniform("u_Uniforms.ViewProjectionMatrix", m_ViewProjectionMatrix);

			m_LinePipeline->DrawIndexed(m_LineIndexCount);
		}
		
		m_LineVertexPointer = m_LineVertexStorage;
		m_LineIndexCount = 0;

		m_TextureSlotIndex = 1;
	}

	void Renderer::RenderLine(const glm::vec3& position1, const glm::vec4& color1, const glm::vec3& position2, const glm::vec4& color2)
	{
		if (m_LineIndexCount >= s_MaxLines)
			FlushLines();

		m_LineVertexPointer->Position = position1;
		m_LineVertexPointer->Color = color1;
		m_LineVertexPointer++;

		m_LineVertexPointer->Position = position2;
		m_LineVertexPointer->Color = color2;
		m_LineVertexPointer++;

		m_LineIndexCount += 2;
	}

	void Renderer::SetViewportSize(uint32 width, uint32 height)
	{
		if (m_ViewportWidth != width || m_ViewportHeight != height)
		{
			// resize framebuffers here

			m_ViewportWidth = width;
			m_ViewportHeight = height;
		}
	}

}