#include "GolfDashPCH.h"
#include "GolfDash.h"

namespace gd {

	GolfDash::GolfDash()
	{
		s_Instance = this;

		WindowConfig windowConfig;
		windowConfig.Width = 1920;
		windowConfig.Height = 1080;
		windowConfig.Title = "Golf Dash";
		windowConfig.Fullscreen = false;

		m_Window = Ref<Window>::Create(windowConfig);
		m_Window->CreateContext();
		m_Window->AddCloseCallback([this]()
		{
			m_Running = false;
		});

		m_Mouse = Ref<Mouse>::Create(m_Window);

		// Create level
		m_Level = Ref<Level>::Create(m_Window->GetFramebufferWidth(), m_Window->GetFramebufferHeight());
		m_Window->AddFramebufferSizeCallback([this](uint32 width, uint32 height) { m_Level->SetViewportSize(width, height); });
		m_Level->SetViewportSize(m_Window->GetFramebufferWidth(), m_Window->GetFramebufferHeight());
	}

	void GolfDash::Run()
	{
		float deltaTime = 0.0f;
		float lastTime = 0.0f;

		m_Window->ShowWindow();
		while (m_Running)
		{
			float time = m_Window->GetTime();
			deltaTime = time - lastTime;
			lastTime = time;

			m_Mouse->OnUpdate();
			m_Window->PollEvents();
			m_Level->OnUpdate(time, deltaTime);
			m_Window->SwapBuffers();
		}
	}

	gd::GolfDash* GolfDash::s_Instance;

}