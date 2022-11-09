#pragma once

#include "Camera.h"

#include "Shader.h"
#include "Pipeline.h"
#include "Framebuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

namespace gd {

	class Renderer : public ReferenceCounted
	{
	public:
		Renderer();
		virtual ~Renderer();

		void BeginFrame(const Camera& camera);
		void EndFrame() const;

		void RenderQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, Ref<Texture> texture, float tilingFactor = 1.0f, bool waveEffect = false);
		void FlushQuads();

		void RenderLine(const glm::vec3& position1, const glm::vec4& color1, const glm::vec3& position2, const glm::vec4& color2);
		void FlushLines();

		void SetViewportSize(uint32 width, uint32 height);
	private:
		Camera m_Camera;
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		uint32 m_ViewportWidth = 0;
		uint32 m_ViewportHeight = 0;

		// Shaders
		Ref<Shader> m_QuadShader;
		Ref<Shader> m_LineShader;
		Ref<Shader> m_CompositeShader;

		// Pipelines
		Ref<Pipeline> m_QuadPipeline;
		Ref<Pipeline> m_LinePipeline;
		Ref<Pipeline> m_CompositePipeline;

		// Framebuffers
		Ref<Framebuffer> m_GeometryFramebuffer;
		Ref<Framebuffer> m_CompositeFramebuffer;

		// Constants
		static const uint32 s_MaxQuads = 1000;
		static const uint32 s_MaxLines = 1000;
		static const uint32 s_MaxQuadVertices = s_MaxQuads * 4;
		static const uint32 s_MaxQuadIndices = s_MaxQuads * 6;
		static const uint32 s_MaxLineVertices = s_MaxLines * 2;
		static const uint32 s_MaxTextureSlots = 32;

		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			float TilingFactor;
			float Wave;
		};

		// Quads
		Ref<VertexBuffer> m_QuadVertexBuffer;
		Ref<IndexBuffer> m_QuadIndexBuffer;
		QuadVertex* m_QuadVertexStorage = nullptr;
		QuadVertex* m_QuadVertexPointer = nullptr;
		glm::vec4 m_QuadVertexPositions[4];
		glm::vec2 m_QuadTextureCoords[4];
		uint32 m_QuadIndexCount = 0;

		struct LineVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};

		// Lines
		Ref<VertexBuffer> m_LineVertexBuffer;
		Ref<IndexBuffer> m_LineIndexBuffer;
		LineVertex* m_LineVertexStorage = nullptr;
		LineVertex* m_LineVertexPointer = nullptr;
		uint32 m_LineIndexCount = 0;

		std::array<Ref<Texture>, s_MaxTextureSlots> m_TextureSlots;
		uint32 m_TextureSlotIndex = 1;

		Ref<VertexBuffer> m_FullscreenQuadVertexBuffer;
		Ref<IndexBuffer> m_FullscreenQuadIndexBuffer;
	};

}