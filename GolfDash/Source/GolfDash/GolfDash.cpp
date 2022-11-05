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

	}

	void GolfDash::OnUpdate()
	{

	}

}