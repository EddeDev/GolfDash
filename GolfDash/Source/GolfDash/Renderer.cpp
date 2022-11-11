#include "GolfDashPCH.h"
#include "Renderer.h"

#include "GolfDash.h"

#include "glad/glad.h"

namespace gd {

	Renderer::Renderer()
	{
		FramebufferConfig geometryFramebufferConfig;
		geometryFramebufferConfig.Attachments = { { TextureFormat::RGBA }, { TextureFormat::Depth24Stencil8 } };
		geometryFramebufferConfig.Width = m_ViewportWidth;
		geometryFramebufferConfig.Height = m_ViewportHeight;

		m_GeometryFramebuffer = Ref<Framebuffer>::Create(geometryFramebufferConfig);

		// Quad
		{
			m_QuadShader = Ref<Shader>::Create("Assets/Shaders/Quad_Vertex.glsl", "Assets/Shaders/Quad_Fragment.glsl");

			PipelineConfig pipelineConfig;
			pipelineConfig.Topology = PrimitiveTopology::Triangles;
			pipelineConfig.Framebuffer = m_GeometryFramebuffer;
			pipelineConfig.Shader = m_QuadShader;
			pipelineConfig.InputLayout = {
				{ "a_Position", ShaderDataType::Vec3 },
				{ "a_Color", ShaderDataType::Vec4 },
				{ "a_TexCoord", ShaderDataType::Vec2 },
				{ "a_TexIndex", ShaderDataType::Float },
				{ "a_TilingFactor", ShaderDataType::Float },
				{ "a_Wave", ShaderDataType::Float },
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
			pipelineConfig.Framebuffer = m_GeometryFramebuffer;
			pipelineConfig.Shader = m_LineShader;
			pipelineConfig.InputLayout = {
				{ "a_Position", ShaderDataType::Vec3 },
				{ "a_Color", ShaderDataType::Vec4 }
			};
			pipelineConfig.LineWidth = 5.0f;
			pipelineConfig.AntialiasedLines = true;
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

			FramebufferConfig framebufferConfig;
			framebufferConfig.SwapchainTarget = true;
			m_CompositeFramebuffer = Ref<Framebuffer>::Create(framebufferConfig);

			PipelineConfig pipelineConfig;
			pipelineConfig.Topology = PrimitiveTopology::Triangles;
			pipelineConfig.Framebuffer = m_CompositeFramebuffer;
			pipelineConfig.Shader = m_CompositeShader;
			pipelineConfig.InputLayout = {
				{ "a_Position", ShaderDataType::Vec3 },
				{ "a_TexCoord", ShaderDataType::Vec2 }
			};
			m_CompositePipeline = Ref<Pipeline>::Create(pipelineConfig);
		}

		struct FullscreenQuadVertex
		{
			glm::vec3 Position;
			glm::vec2 TexCoord;
		};

		FullscreenQuadVertex vertices[4];
		vertices[0] = { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } };
		vertices[1] = { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } };
		vertices[2] = { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } };
		vertices[3] = { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } };

		uint32 indices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		m_FullscreenQuadVertexBuffer = Ref<VertexBuffer>::Create(vertices, 4 * sizeof(FullscreenQuadVertex));
		m_FullscreenQuadIndexBuffer = Ref<IndexBuffer>::Create(indices, 6 * sizeof(uint32));

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

		m_QuadVertexPointer = m_QuadVertexStorage;
		m_QuadIndexCount = 0;

		m_LineVertexPointer = m_LineVertexStorage;
		m_LineIndexCount = 0;

		m_TextureSlotIndex = 1;
	}

	void Renderer::EndFrame() const
	{
		m_GeometryFramebuffer->Bind();

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
			m_QuadShader->SetUniform("u_Time", GolfDash::Get().GetWindow()->GetTime());

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

		// Post-processing
		{
			m_CompositeFramebuffer->Bind();

			m_FullscreenQuadVertexBuffer->Bind();
			m_CompositePipeline->Bind();
			m_FullscreenQuadIndexBuffer->Bind();

			m_GeometryFramebuffer->BindAttachment();

			m_CompositeShader->Bind();
			m_CompositeShader->SetUniform("u_Texture", m_GeometryFramebuffer->GetAttachmentID());
			m_CompositeShader->SetUniform("u_Time", GolfDash::Get().GetWindow()->GetTime());
			m_CompositeShader->SetUniform("u_Wave", m_CompositeWave);

			m_CompositePipeline->DrawIndexed(m_FullscreenQuadIndexBuffer->GetCount());
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

	void Renderer::RenderQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, Ref<Texture> texture, float tilingFactor, bool waveEffect)
	{
		if (m_QuadIndexCount >= s_MaxQuadIndices)
			FlushQuads();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

		float textureIndex = 0.0f;
		if (texture)
		{
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
		}

		for (size_t i = 0; i < 4; i++)
		{
			m_QuadVertexPointer->Position = transform * m_QuadVertexPositions[i];
			m_QuadVertexPointer->Color = color;
			m_QuadVertexPointer->TexCoord = m_QuadTextureCoords[i];
			m_QuadVertexPointer->TexIndex = textureIndex;
			m_QuadVertexPointer->TilingFactor = tilingFactor;
			m_QuadVertexPointer->Wave = waveEffect ? 1.0f : 0.0f;
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
			m_GeometryFramebuffer->Resize(width, height);
			m_CompositeFramebuffer->Resize(width, height);

			// resize framebuffers here
			m_ViewportWidth = width;
			m_ViewportHeight = height;
		}
	}

}