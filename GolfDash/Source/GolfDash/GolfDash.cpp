#include "GolfDashPCH.h"
#include "GolfDash.h"

#include "Shader.h"

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
	}

	void GolfDash::OnUpdate()
	{

	}

	void GolfDash::SetViewportSize(uint32 width, uint32 height)
	{
		if (width != m_ViewportWidth || height != m_ViewportHeight)
		{
			glViewport(0, 0, width, height);

			m_ViewportWidth = width;
			m_ViewportWidth = height;
		}
	}

}