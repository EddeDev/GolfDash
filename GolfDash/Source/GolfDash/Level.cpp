#include "GolfDashPCH.h"
#include "Level.h"

namespace gd {

	Level::Level(const LevelSpecification& specification)
		: m_Obstacles(specification.Obstacles), m_BackgroundTexture(specification.BackgroundTexture)
	{
		m_Ball = Ball(this, specification.BallPosition);
		m_Hole = Hole(this, specification.HolePosition);

		m_LogoTexture = Texture::Create("Assets/Textures/Logo_Text.psd");
		m_HoleInOneTexture = Texture::Create("Assets/Textures/HoleInOne_Text.psd");

		m_FeedbackTextures[1] = {
			Texture::Create("Assets/Textures/Feedback_Amazing.psd"),
			Texture::Create("Assets/Textures/Feedback_Excellent.psd"),
			Texture::Create("Assets/Textures/Feedback_Super.psd")
		};

		m_FeedbackTextures[2] = {

			Texture::Create("Assets/Textures/Feedback_Great.psd"),
			Texture::Create("Assets/Textures/Feedback_ThatsGreat.psd")
		};

		m_FeedbackTextures[3] = {
			Texture::Create("Assets/Textures/Feedback_KeepItUp.psd"),
			Texture::Create("Assets/Textures/Feedback_Fine.psd")
		};

		m_FeedbackTextures[4] = {
			Texture::Create("Assets/Textures/Feedback_KeepItUp.psd"),
		};

		std::string levelTexturePath = "Assets/Textures/Level" + std::to_string(specification.Index) + "_Text.psd";
		m_LevelTexture = Texture::Create(levelTexturePath);
	}

	void Level::OnUpdate(float time, float deltaTime)
	{
		m_Renderer->BeginFrame(m_Camera);

		RenderBackground();
		RenderLevelText();
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
				m_Renderer->SetCompositeWave(t * 0.005f);

			if (m_Ball.GetStrokes() == 1)
			{
				glm::vec2 scale = glm::normalize(glm::vec2((float)m_HoleInOneTexture->GetWidth(), (float)m_HoleInOneTexture->GetHeight()));
				scale *= 3.0f;
				scale *= t;

				// TODO: hue shift?

				float tilingFactor = 1.0f; // glm::length(scale);

				m_Renderer->RenderQuad({ 0.0f, 0.0f, -0.01f }, scale, glm::vec4(t), m_HoleInOneTexture, tilingFactor);
			}

			if (!m_CurrentFeedbackTexture)
				m_CurrentFeedbackTexture = GetRandomFeedbackTexture();
		
			if (m_CurrentFeedbackTexture)
			{
				glm::vec2 scale = glm::normalize(glm::vec2((float)m_CurrentFeedbackTexture->GetWidth(), (float)m_CurrentFeedbackTexture->GetHeight()));
				scale *= t;

				m_Renderer->RenderQuad({ 0.0f, -0.35f, -0.01f }, scale, glm::vec4(t * 0.8f, t * 0.8f, 0.6f - t * 0.8f, 1.0f), m_CurrentFeedbackTexture, 1.0f);
			}
		}

		m_Renderer->EndFrame();
	}

	void Level::Clear()
	{
		m_CurrentFeedbackTexture = nullptr;
	}

	Ref<Texture> Level::GetRandomFeedbackTexture() const
	{
		static std::random_device device;
		static std::mt19937 gen(device());

		uint32 feedbackTextureIndex = glm::clamp(m_Ball.GetStrokes(), static_cast<uint32>(1), static_cast<uint32>(m_FeedbackTextures.size()) - 1);
		std::vector<Ref<Texture>> feedbacks = m_FeedbackTextures.at(feedbackTextureIndex);

		std::uniform_int_distribution<> dist(0, feedbacks.size() - 1);
		return feedbacks[dist(gen)];
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

	void Level::RenderLevelText()
	{
		if (!m_LevelTexture)
		{
			std::cout << "Texture is null!" << std::endl;
			return;
		}

		glm::vec2 scale = glm::normalize(glm::vec2((float)m_LevelTexture->GetWidth(), (float)m_LevelTexture->GetHeight()));
		scale *= 0.5f;

		glm::vec2 position = { 0.0f, 1.0f + 0.05f - (scale.y * 0.5f) };
	
		m_Renderer->RenderQuad(glm::vec3(position, -0.15f), scale, glm::vec4(1.0f), m_LevelTexture);

		// render shadow
		glm::vec2 shadowOffset = { 0.0f, GD_SHADOW_OFFSET_Y };
		m_Renderer->RenderQuad(glm::vec3(position + shadowOffset, -0.16f), scale, glm::vec4(0.0f, 0.0f, 0.0f, GD_SHADOW_ALPHA), m_LevelTexture);
	}

	void Level::RenderLogo()
	{
		glm::vec2 logoScale = glm::normalize(glm::vec2((float)m_LogoTexture->GetWidth(), (float)m_LogoTexture->GetHeight()));
		logoScale *= 0.5f;

		glm::vec2 logoPosition = { -m_Camera.GetAspectRatio() + (logoScale.x * 0.5f), 1.0f + 0.05f - (logoScale.y * 0.5f) };

		m_Renderer->RenderQuad(glm::vec3(logoPosition, -0.15f), logoScale, glm::vec4(1.0f), m_LogoTexture);

		// render shadow
		glm::vec2 shadowOffset = { 0.0f, GD_SHADOW_OFFSET_Y };
		m_Renderer->RenderQuad(glm::vec3(logoPosition + shadowOffset, -0.16f), logoScale, glm::vec4(0.0f, 0.0f, 0.0f, GD_SHADOW_ALPHA), m_LogoTexture);
	}

	void Level::RenderObstacles()
	{
		for (auto& obstacle : m_Obstacles)
		{
			m_Renderer->RenderQuad(glm::vec3(obstacle.Position, -0.1f), obstacle.Scale, obstacle.Color, obstacle.Texture);

			// render shadow
			glm::vec2 shadowOffset = { 0.0f, GD_SHADOW_OFFSET_Y };
			m_Renderer->RenderQuad(glm::vec3(obstacle.Position + shadowOffset, -0.11f), obstacle.Scale, glm::vec4(0.0f, 0.0f, 0.0f, GD_SHADOW_ALPHA), obstacle.Texture);
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
				level->Clear();

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