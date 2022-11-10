#include "GolfDashPCH.h"
#include "Level.h"

namespace gd {

	Level::Level(const LevelSpecification& specification)
		: m_Obstacles(specification.Obstacles), m_BackgroundTexture(specification.BackgroundTexture)
	{
		m_Ball = Ball(this, specification.BallPosition);
		m_Hole = Hole(this, specification.HolePosition);

		m_LogoTexture = Ref<Texture>::Create("Assets/Textures/GUI/Logo_Text.psd");
		m_HoleInOneTexture = Ref<Texture>::Create("Assets/Textures/GUI/HoleInOne_Text.psd");
	
		m_FeedbackTextures.push_back(Ref<Texture>::Create("Assets/Textures/GUI/Feedback_Amazing.psd"));
	}

	void Level::OnUpdate(float time, float deltaTime)
	{
		m_Renderer->BeginFrame(m_Camera);

		RenderBackground();
		RenderLogo();
		RenderObstacles();
		m_Ball.OnUpdate(time, deltaTime);
		m_Hole.OnUpdate(time);

		if (m_Ball.IsInHole())
		{
			float t = glm::clamp(m_Ball.GetTimeInHole() * 3.0f, 0.0f, 1.0f);

			if (m_Ball.IsReadyForNextLevel())
				m_Renderer->SetCompositeWave(0.0f);
			else
				m_Renderer->SetCompositeWave(t * 0.02f);

			if (m_Ball.GetStrokes() == 1)
			{
				glm::vec2 scale = glm::normalize(glm::vec2((float)m_HoleInOneTexture->GetWidth(), (float)m_HoleInOneTexture->GetHeight()));
				scale *= 3.0f;
				scale *= t;

				// TODO: hue shift?

				float tilingFactor = 1.0f; // glm::length(scale);

				m_Renderer->RenderQuad({ 0.0f, 0.0f, -0.01f }, scale, glm::vec4(t), m_HoleInOneTexture, tilingFactor);

				{
					Ref<Texture> feedback = m_FeedbackTextures[0];

					glm::vec2 scale = glm::normalize(glm::vec2((float)feedback->GetWidth(), (float)feedback->GetHeight()));
					scale *= t;

					m_Renderer->RenderQuad({ 0.0f, -0.35f, -0.01f }, scale, glm::vec4(t), feedback, tilingFactor);
				}
			}
		}

		m_Renderer->EndFrame();
	}

	void Level::SetViewportSize(uint32 width, uint32 height)
	{
		m_Camera.SetViewportSize(width, height);
	}

	void Level::RenderBackground()
	{
		if (!m_BackgroundTexture)
			return;

		float size = 3.0f;
		for (float x = -size; x <= size; x += 1.0f)
		{
			for (float y = -size; y <= size; y += 1.0f)
			{
				m_Renderer->RenderQuad({ x, y, -0.2f }, glm::vec2(1.0f), glm::vec4(1.0f), m_BackgroundTexture);
			}
		}
	}

	void Level::RenderLogo()
	{
		glm::vec2 logoScale = glm::normalize(glm::vec2((float)m_LogoTexture->GetWidth(), (float)m_LogoTexture->GetHeight()));
		logoScale *= 0.5f;

		glm::vec2 logoPosition = { -m_Camera.GetAspectRatio() + (logoScale.x * 0.5f), 1.0f + 0.05f - (logoScale.y * 0.5f) };

		m_Renderer->RenderQuad(glm::vec3(logoPosition, -0.1f), logoScale, glm::vec4(1.0f), m_LogoTexture);
	}

	void Level::RenderObstacles()
	{
		for (auto& obstacle : m_Obstacles)
		{
			m_Renderer->RenderQuad(glm::vec3(obstacle.Position, -0.1f), obstacle.Scale, obstacle.Color, obstacle.Texture);
		}
	}

	LevelManager::LevelManager()
	{
		m_Renderer = Ref<Renderer>::Create();
	}

	LevelManager::~LevelManager()
	{
		m_LevelBindings.clear();
	}

	void LevelManager::OnUpdate(float time, float deltaTime)
	{
		Ref<Level> level = GetActiveLevel();
		if (!level)
		{
			std::cerr << "Level is null!" << std::endl;
			return;
		}

		level->OnUpdate(time, deltaTime);

		if (level->GetBall().HasWon())
		{
			uint32 nextLevelTypeIndex = static_cast<uint32>(m_ActiveLevelType) + 1;
			LevelType nextLevelType = static_cast<LevelType>(nextLevelTypeIndex);

			Ref<Level> nextLevel = GetLevel(nextLevelType);
			if (nextLevel)
			{
				// clear last level

				SetActiveLevelType(nextLevelType);
			}
		}
	}

	void LevelManager::SetViewportSize(uint32 width, uint32 height)
	{
		if (width == 0 || height == 0)
			return;

		if (width != m_ViewportWidth || height != m_ViewportHeight)
		{
			m_Renderer->SetViewportSize(width, height);

			for (auto& [levelType, level] : m_LevelBindings)
				level->SetViewportSize(width, height);

			m_ViewportWidth = width;
			m_ViewportHeight = height;
		}
	}

	void LevelManager::BindLevel(LevelType type, Ref<Level> level, bool setActive)
	{
		if (m_LevelBindings.find(type) != m_LevelBindings.end())
		{
			std::cerr << "Level already exists in the registry!" << std::endl;
			return;
		}

		m_LevelBindings[type] = level;

		if (setActive)
			SetActiveLevelType(type);
	}

	void LevelManager::SetActiveLevelType(LevelType type)
	{
		m_ActiveLevelType = type;
		
		Ref<Level> level = GetActiveLevel();
		if (!level)
		{
			std::cerr << "Level is null!" << std::endl;
			return;
		}

		level->SetRenderer(m_Renderer);
	}

	Ref<Level> LevelManager::GetActiveLevel() const
	{
		return GetLevel(m_ActiveLevelType);
	}

	Ref<Level> LevelManager::GetLevel(LevelType type) const
	{
		if (m_LevelBindings.find(type) == m_LevelBindings.end())
			return nullptr;

		return m_LevelBindings.at(type);
	}

}