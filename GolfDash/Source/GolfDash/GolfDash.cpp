#include "GolfDashPCH.h"
#include "GolfDash.h"

namespace gd {

#define PRINT(message) std::cout << message << std::endl

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

		m_Mouse = Ref<Mouse>::Create(m_Window);
		m_Renderer = Ref<Renderer>::Create();

		SetViewportSize(m_Window->GetFramebufferWidth(), m_Window->GetFramebufferHeight());
	}

	void GolfDash::Run()
	{
		OnInit();

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

			m_Renderer->BeginFrame();
			OnUpdate(time, deltaTime);
			m_Renderer->EndFrame();
			
			m_Window->SwapBuffers();
		}
	}

	void GolfDash::OnInit()
	{
		m_GrassTexture = Ref<Texture>::Create("Assets/Textures/Grass.png");
		m_BallTexture = Ref<Texture>::Create("Assets/Textures/Ball.png");
		m_HoleTexture = Ref<Texture>::Create("Assets/Textures/Hole.png");

		m_HolePosition = { 0.5f, 0.5f };
	}

	void GolfDash::OnUpdate(float time, float deltaTime)
	{
		m_AspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;
		m_ViewMatrix = glm::inverse(glm::mat4(1.0f));
		m_ProjectionMatrix = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
		m_Renderer->SetViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix);

		RenderBackground();
		RenderBall(time, deltaTime);
		RenderHole();
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

	void GolfDash::RenderBall(float time, float deltaTime)
	{
		glm::vec2 orthoMousePos = m_Mouse->GetMouseOrthoPosition(m_ViewMatrix, m_ProjectionMatrix);
		glm::vec2 ballScale = glm::vec2(0.1f);

		if (m_Mouse->GetMouseButton(GD_MOUSE_BUTTON_LEFT))
		{
			if (glm::epsilonEqual(orthoMousePos, m_BallPosition, ballScale * 0.5f) == glm::bvec2(true))
			{
				if (m_BallVelocity.x == 0.0f && m_BallVelocity.y == 0.0f)
					m_IsDragging = true;
			}
		}

		if (m_IsDragging)
		{
			glm::vec2 begin = orthoMousePos;
			glm::vec2 end = m_BallPosition;

			m_Renderer->RenderLine({ begin, -0.1f }, glm::vec4(0.0f), { end, -0.1f }, glm::vec4(1.0f));
		}

		if (m_Mouse->GetMouseButtonUp(GD_MOUSE_BUTTON_LEFT))
		{
			if (m_IsDragging)
			{
				const float maxDistance = 2.0f;

				float distance = glm::distance(orthoMousePos, m_BallPosition);
				if (distance <= ballScale.x)
				{
					PRINT("Ball was dragged within it's boundaries (no force applied!)");
				}
				else if (distance >= maxDistance)
				{
					PRINT("Reached the max distance!");
				}
				else
				{
					PRINT("Stopped dragging, adding force! distance: " << distance);

					m_InitialBallVelocity = glm::normalize(m_BallPosition - orthoMousePos);
					m_InitialBallVelocity *= glm::sqrt(distance);
					PRINT("Initial velocity: " << m_InitialBallVelocity.x << ", " << m_InitialBallVelocity.y);
					m_BallVelocity = m_InitialBallVelocity;

					m_BallDirection.x = m_BallVelocity.x / glm::abs(m_BallVelocity.x);
					m_BallDirection.y = m_BallVelocity.y / glm::abs(m_BallVelocity.y);

					m_InitialBallMagnitude = glm::length(m_BallVelocity);
					// m_InitialBallMagnitude = glm::min(m_InitialBallMagnitude, 1.0f); // TODO: clamp?
					m_BallMagnitude = m_InitialBallMagnitude;
				}
				m_IsDragging = false;
			}
		}

		m_BallPosition += m_BallVelocity * deltaTime;

		const float epsilon = 0.0001f;
		if (m_BallVelocity.x > epsilon || m_BallVelocity.x < -epsilon || m_BallVelocity.y > epsilon || m_BallVelocity.y < -epsilon)
		{
			const float friction = 0.5f;

			if (m_BallMagnitude > 0.0f)
				m_BallMagnitude -= friction * deltaTime;
			if (m_BallMagnitude < 0.0f)
				m_BallMagnitude = 0.0f;
			// PRINT("Magnitude: " << m_BallMagnitude);

			m_BallVelocity.x = (m_BallMagnitude / m_InitialBallMagnitude) * glm::abs(m_InitialBallVelocity.x) * m_BallDirection.x;
			m_BallVelocity.y = (m_BallMagnitude / m_InitialBallMagnitude) * glm::abs(m_InitialBallVelocity.y) * m_BallDirection.y;
		}
		else
		{
			// Reset ball velocity
			m_InitialBallVelocity = { 0.0f, 0.0f };
			m_BallVelocity = { 0.0f, 0.0f };
		}

		if (m_BallPosition.x >= m_AspectRatio - (ballScale.x * 0.5f))
		{
			m_BallVelocity = { -m_BallVelocity.x, m_BallVelocity.y };
			m_BallDirection.x = -1.0f;
		}

		if (m_BallPosition.x <= -m_AspectRatio + (ballScale.x * 0.5f))
		{
			m_BallVelocity = { -m_BallVelocity.x, m_BallVelocity.y };
			m_BallDirection.x = 1.0f;
		}

		if (m_BallPosition.y >= 1.0f - (ballScale.x * 0.5f))
		{
			m_BallVelocity = { m_BallVelocity.x, -m_BallVelocity.y };
			m_BallDirection.y = -1.0f;
		}

		if (m_BallPosition.y <= -1.0f + (ballScale.x * 0.5f))
		{
			m_BallVelocity = { m_BallVelocity.x, -m_BallVelocity.y };
			m_BallDirection.y = 1.0f;
		}

		if (m_BallMagnitude < 1.0f)
		{
			float size = 0.35f;
			if (glm::epsilonEqual(m_BallPosition, m_HolePosition, ballScale * size) == glm::bvec2(true))
			{
				if (!m_IsBallInHole)
				{
					// Reset ball velocity
					m_InitialBallVelocity = { 0.0f, 0.0f };
					m_BallVelocity = { 0.0f, 0.0f };

					m_IsBallInHole = true;
				}

				m_TimeInHole += deltaTime;
				if (m_TimeInHole >= 2.0f)
				{
					m_IsBallInHole = false;
					m_TimeInHole = 0.0f;

					// temp
					m_BallPosition = { -0.5f, -0.5f };
				}
			}
		}

		if (!m_IsBallInHole)
			m_Renderer->RenderQuad({ m_BallPosition, -0.1f }, ballScale, { 1.0f, 1.0f, 1.0f, 1.0f }, m_BallTexture);
	}

	void GolfDash::RenderHole()
	{
		m_Renderer->RenderQuad({ m_HolePosition, -0.1f }, glm::vec2(0.1f), { 1.0f, 1.0f, 1.0f, 1.0f }, m_HoleTexture);
	}

	void GolfDash::SetViewportSize(uint32 width, uint32 height)
	{
		if (width != m_ViewportWidth || height != m_ViewportHeight)
		{
			m_Renderer->SetViewportSize(width, height);

			m_ViewportWidth = width;
			m_ViewportHeight = height;
		}
	}

}