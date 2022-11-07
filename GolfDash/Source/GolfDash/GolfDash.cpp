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

		m_Window = Ref<Window>::Create(windowConfig);
		m_Window->CreateContext();
		m_Window->AddCloseCallback([this]()
		{
			m_Running = false;
		});
		m_Window->AddFramebufferSizeCallback([this](uint32 width, uint32 height) { SetViewportSize(width, height); });

		m_Mouse = Ref<Mouse>::Create(m_Window);
		m_Renderer = Ref<Renderer>::Create();

		SetViewportSize(m_Window->GetFramebufferWidth(), m_Window->GetFramebufferHeight());
		m_Camera = Camera(m_ViewportWidth, m_ViewportHeight);
	}

	void GolfDash::Run()
	{
		m_GrassTexture = Ref<Texture>::Create("Assets/Textures/Grass.png");
		
		m_Ball = Ref<Ball>::Create();
		m_Hole = Ref<Hole>::Create(glm::vec2(0.5f, 0.5f));

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

			m_Renderer->BeginFrame(m_Camera);
			{
				RenderBackground();
				m_Ball->OnUpdate(time, deltaTime);
				m_Hole->OnUpdate();
			}
			m_Renderer->EndFrame();
			
			m_Window->SwapBuffers();
		}
	}

	void GolfDash::RenderBackground()
	{
		float size = 3.0f;
		for (float x = -size; x <= size; x += 1.0f)
		{
			for (float y = -size; y <= size; y += 1.0f)
			{
				m_Renderer->RenderQuad({ x, y, -0.2f }, glm::vec2(1.0f), { 1.0f, 1.0f, 1.0f, 1.0f }, m_GrassTexture);
			}
		}
	}

	void GolfDash::SetViewportSize(uint32 width, uint32 height)
	{
		if (width != m_ViewportWidth || height != m_ViewportHeight)
		{
			m_Camera.SetViewportSize(width, height);
			m_Renderer->SetViewportSize(width, height);

			m_ViewportWidth = width;
			m_ViewportHeight = height;
		}
	}

	gd::GolfDash* GolfDash::s_Instance;

}