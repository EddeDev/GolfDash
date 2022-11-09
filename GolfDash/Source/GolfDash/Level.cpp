#include "GolfDashPCH.h"
#include "Level.h"

namespace gd {

	Level::Level()
	{
		m_Renderer = Ref<Renderer>::Create();
		m_GrassTexture = Ref<Texture>::Create("Assets/Textures/NewStyle/BG_Tiles_Default.psd");

		m_Ball = Ball(this, { -1.0f, 0.0f });
		m_Hole = Hole(this, { 1.0f, 0.0f });

		// TODO List: 
		// - Post processing effects
		// - Boost pads
		// - Level specifications

#if 0
		Obstacle& obstacle = m_Obstacles.emplace_back();
		obstacle.Position = { -0.5f, -0.5f };
		obstacle.Texture = Ref<Texture>::Create("Assets/Textures/Obstacle_1.png");
		// obstacle.Color = { 0.41f, 0.28f, 0.19f, 1.0f };
		obstacle.Scale *= 0.5f;
#endif

		Ref<Texture> obstacle1Texture = Ref<Texture>::Create("Assets/Textures/NewStyle/Obstacle.psd");
		Ref<Texture> obstacle2Texture = Ref<Texture>::Create("Assets/Textures/Obstacle_2.png");

		Obstacle& obstacle1 = m_Obstacles.emplace_back();
		obstacle1.Position = { 0.0f, -0.3f };
		obstacle1.Texture = obstacle2Texture;
		obstacle1.Scale.x = 0.5f;
		obstacle1.Scale.y = 0.5f;

		Obstacle& obstacle2 = m_Obstacles.emplace_back();
		obstacle2.Position = { 0.0f, 0.3f };
		obstacle2.Texture = obstacle2Texture;
		obstacle2.Scale.x = 0.5f;
		obstacle2.Scale.y = 0.5f;

		Obstacle& obstacle3 = m_Obstacles.emplace_back();
		obstacle3.Position = { -0.5f, 0.0f };
		obstacle3.Texture = obstacle1Texture;
		obstacle3.Scale.x = 0.3f;
		obstacle3.Scale.y = 0.3f;

		Obstacle& obstacle4 = m_Obstacles.emplace_back();
		obstacle4.Position = { 0.5f, 0.0f };
		obstacle4.Texture = obstacle1Texture;
		obstacle4.Scale.x = 0.3f;
		obstacle4.Scale.y = 0.3f;

		Obstacle& obstacle5 = m_Obstacles.emplace_back();
		obstacle5.Position = { -1.5f, 0.75f };
		obstacle5.Texture = obstacle1Texture;
		obstacle5.Scale.x = 0.3f;
		obstacle5.Scale.y = 0.3f;

		Obstacle& obstacle6 = m_Obstacles.emplace_back();
		obstacle6.Position = { 1.5f, 0.75f };
		obstacle6.Texture = obstacle1Texture;
		obstacle6.Scale.x = 0.3f;
		obstacle6.Scale.y = 0.3f;

		Obstacle& obstacle7 = m_Obstacles.emplace_back();
		obstacle7.Position = { -1.5f, -0.75f };
		obstacle7.Texture = obstacle1Texture;
		obstacle7.Scale.x = 0.3f;
		obstacle7.Scale.y = 0.3f;

		Obstacle& obstacle8 = m_Obstacles.emplace_back();
		obstacle8.Position = { 1.5f, -0.75f };
		obstacle8.Texture = obstacle1Texture;
		obstacle8.Scale.x = 0.3f;
		obstacle8.Scale.y = 0.3f;
	}

	void Level::OnUpdate(float time, float deltaTime)
	{
		m_Renderer->BeginFrame(m_Camera);

		RenderBackground();
		RenderObstacles();
		m_Ball.OnUpdate(time, deltaTime);
		m_Hole.OnUpdate(time);

		m_Renderer->EndFrame();
	}

	void Level::SetViewportSize(uint32 width, uint32 height)
	{
		if (width == 0 || height == 0)
			return;

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