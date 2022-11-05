#include "GolfDashPCH.h"
#include "GolfDash.h"

#include "glad/glad.h"

#include <iostream>

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

		m_ViewportWidth = m_Window->GetFramebufferWidth();
		m_ViewportHeight = m_Window->GetFramebufferHeight();
		m_Window->AddFramebufferSizeCallback([this](uint32 width, uint32 height) { OnFramebufferSizeCallback(width, height); });
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

	void GolfDash::OnFramebufferSizeCallback(uint32 width, uint32 height)
	{
		glViewport(0, 0, width, height);

		m_ViewportWidth = width;
		m_ViewportWidth = height;
	}

}