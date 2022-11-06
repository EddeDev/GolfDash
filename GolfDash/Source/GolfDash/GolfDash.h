#pragma once

#include "Window.h"
#include "Mouse.h"

#include "Shader.h"
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

namespace gd {

	class GolfDash
	{
	public:
		GolfDash();

		void Run();
	private:
		void OnInit();
		void OnUpdate(float time, float deltaTime);

		void SetViewportSize(uint32 width, uint32 height);
	private:
		Ref<Window> m_Window;
		Ref<Mouse> m_Mouse;
		bool m_Running = true;

		Ref<Shader> m_DefaultShader;
		Ref<Pipeline> m_DefaultPipeline;

		Ref<Shader> m_CompositeShader;
		Ref<Pipeline> m_CompositePipeline;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		Ref<Texture> m_GrassTexture;
		Ref<Texture> m_BallTexture;
		Ref<Texture> m_HoleTexture;

		glm::vec2 m_HolePosition = glm::vec2(0.0f);

		glm::vec2 m_BallPosition = glm::vec2(0.0f);
		glm::vec2 m_InitialBallVelocity = glm::vec2(0.0f);
		glm::vec2 m_BallVelocity = glm::vec2(0.0f);
		bool m_IsDragging = false;
		bool m_IsBallInHole = false;
		float m_TimeInHole = 0.0f;

		uint32 m_ViewportWidth = 0;
		uint32 m_ViewportHeight = 0;
	};

}