#include "GolfDashPCH.h"
#include "Level.h"

namespace gd {

	Level::Level()
	{
		m_Renderer = Ref<Renderer>::Create();
		m_GrassTexture = Ref<Texture>::Create("Assets/Textures/Grass.png");

		m_Ball = Ball(this);
		m_Hole = Hole(this, { 0.5f, 0.5f });

		// TODO List: 
		// - Framebuffers
		// - Post processing effects
		// - Obstacle textures
		// - Boost pads
		// - Level specifications

		Obstacle& obstacle = m_Obstacles.emplace_back();
		obstacle.Position = { -0.5f, -0.5f };
		obstacle.Color = { 0.41f, 0.28f, 0.19f, 1.0f };
		obstacle.Scale *= 0.5f;

		Obstacle& obstacle2 = m_Obstacles.emplace_back();
		obstacle2.Position = { 0.5f, -0.5f };
		obstacle.Color = { 0.41f, 0.28f, 0.19f, 1.0f };
		obstacle2.Scale *= 0.3f;
	}

	void Level::OnUpdate(float time, float deltaTime)
	{
		m_Renderer->BeginFrame(m_Camera);

		RenderBackground();
		RenderObstacles();
		m_Ball.OnUpdate(time, deltaTime);
		m_Hole.OnUpdate();

		m_Renderer->EndFrame();
	}

	void Level::SetViewportSize(uint32 width, uint32 height)
	{
		if (width != m_ViewportWidth || height != m_ViewportHeight)
		{
			m_Renderer->SetViewportSize(width, height);
			m_Camera.SetViewportSize(width, height);

			m_ViewportWidth = width;
			m_ViewportHeight = height;
		}
	}

	void Level::RenderBackground()
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

	void Level::RenderObstacles()
	{
		for (auto& obstacle : m_Obstacles)
		{
			m_Renderer->RenderQuad(glm::vec3(obstacle.Position, -0.1f), obstacle.Scale, obstacle.Color, obstacle.Texture);
		}
	}

}