#include "GolfDashPCH.h"
#include "Level.h"

namespace gd {

	Level::Level(uint32 viewportWidth, uint32 viewportHeight)
		: m_ViewportWidth(viewportWidth), m_ViewportHeight(viewportHeight)
	{
		m_Renderer = Ref<Renderer>::Create();
		m_Camera = Camera(viewportWidth, viewportHeight);

		m_GrassTexture = Ref<Texture>::Create("Assets/Textures/Grass.png");

		m_Ball = Ball(this);
		m_Hole = Hole(this, { 0.5f, 0.5f });
	}

	void Level::OnUpdate(float time, float deltaTime)
	{
		m_Renderer->BeginFrame(m_Camera);

		RenderBackground();
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

}