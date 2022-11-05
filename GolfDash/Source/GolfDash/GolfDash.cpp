#include "GolfDashPCH.h"
#include "GolfDash.h"

#include "glad/glad.h"

namespace gd {

	GolfDash::GolfDash()
	{
		WindowConfig windowConfig;
		windowConfig.Width = 1920;
		windowConfig.Height = 1080;
		windowConfig.Title = "Golf Dash";

		m_Window = Ref<Window>::Create(windowConfig);
		m_Window->CreateContext();
		m_Window->AddCloseCallback([this]()
		{
			m_Running = false;
		});
		m_Window->AddFramebufferSizeCallback([this](uint32 width, uint32 height) { SetViewportSize(width, height); });

		SetViewportSize(m_Window->GetFramebufferWidth(), m_Window->GetFramebufferHeight());
	}

	void GolfDash::Run()
	{
		OnInit();

		m_Window->ShowWindow();
		while (m_Running)
		{
			m_Window->PollEvents();
			OnUpdate();
			m_Window->SwapBuffers();
		}
	}

	void GolfDash::OnInit()
	{
		m_Shader = Ref<Shader>::Create("Assets/Shaders/VertexShader.glsl", "Assets/Shaders/FragmentShader.glsl");

		PipelineConfig pipelineConfig;
		pipelineConfig.Shader = m_Shader;
		pipelineConfig.InputLayout = {
			{ "a_Position", ShaderDataType::Vec3 } /* vec2? */
		};
		m_Pipeline = Ref<Pipeline>::Create(pipelineConfig);

		struct Vertex
		{
			float PositionX;
			float PositionY;
			float PositionZ;
		};

		struct Index
		{
			uint32 V1;
			uint32 V2;
			uint32 V3;
		};

		Vertex vertices[4];
		vertices[0] = { -0.5f, -0.5f, 0.0f };
		vertices[1] = {  0.5f, -0.5f, 0.0f };
		vertices[2] = {  0.5f,  0.5f, 0.0f };
		vertices[3] = { -0.5f,  0.5f, 0.0f };

		Index indices[2];
		indices[0] = { 0, 1, 2 };
		indices[1] = { 2, 3, 0 };

		m_VertexBuffer = Ref<VertexBuffer>::Create(vertices, 4 * sizeof(Vertex));
		m_IndexBuffer = Ref<IndexBuffer>::Create(indices, 2 * sizeof(Index));
	}

	void GolfDash::OnUpdate()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		m_VertexBuffer->Bind();
		m_Pipeline->Bind();
		m_IndexBuffer->Bind();

		m_Shader->Bind();
		m_Pipeline->DrawIndexed(m_IndexBuffer->GetCount());
	}

	void GolfDash::SetViewportSize(uint32 width, uint32 height)
	{
		if (width != m_ViewportWidth || height != m_ViewportHeight)
		{
			glViewport(0, 0, width, height);

			m_ViewportWidth = width;
			m_ViewportHeight = height;
		}
	}

}