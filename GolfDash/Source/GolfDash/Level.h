#pragma once

#include "Renderer.h"
#include "Camera.h"

#include "Ball.h"
#include "Hole.h"
#include "Obstacle.h"
#include "BoostPad.h"

namespace gd {

	struct LevelSpecification
	{
		uint32 Index;

		glm::vec2 BallPosition;
		glm::vec2 HolePosition;

		float GroundFriction = 0.5f;

		std::vector<Obstacle> Obstacles;
		std::vector<BoostPad> BoostPads;

		Ref<Texture> BackgroundTexture;

		// Custom functions
	};

	enum class LevelType : uint32
	{
		None = 0,

		Level1,
		Level2,
		Level3,
		Level4
	};

	// TODO: Level identifiers

	class Level : public ReferenceCounted
	{
	public:
		Level(const LevelSpecification& specification);
		virtual ~Level() {}

		void OnUpdate(float time, float deltaTime);
		void SetViewportSize(uint32 width, uint32 height);

		void SetRenderer(Ref<Renderer> renderer) { m_Renderer = renderer; }
		Ref<Renderer> GetRenderer() const { return m_Renderer; }

		Ball& GetBall() { return m_Ball; }
		const Ball& GetBall() const { return m_Ball; }

		Hole& GetHole() { return m_Hole; }
		const Hole& GetHole() const { return m_Hole; }

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		float GetGroundFriction() const { return m_GroundFriction; }

		std::vector<Obstacle>& GetObstacles() { return m_Obstacles; }
		const std::vector<Obstacle>& GetObstacles() const { return m_Obstacles; }

		std::vector<BoostPad>& GetBoostPads() { return m_BoostPads; }
		const std::vector<BoostPad>& GetBoostPads() const { return m_BoostPads; }
	private:
		void RenderBackground();
		void RenderLevelText();
		void RenderLogo();
		void RenderObstacles();
		void RenderBoostPads();

		void Clear();

		Ref<Texture> GetRandomFeedbackTexture() const;
	private:
		Ref<Renderer> m_Renderer;

		std::unordered_map<uint32, std::vector<Ref<Texture>>> m_FeedbackTextures;

		// Textures
		Ref<Texture> m_BackgroundTexture;
		Ref<Texture> m_LevelTexture;
		Ref<Texture> m_LogoTexture;
		Ref<Texture> m_HoleInOneTexture;
		Ref<Texture> m_CurrentFeedbackTexture;
		Ref<Texture> m_BoostPadTexture;

		// Entities
		Camera m_Camera;
		Ball m_Ball;
		Hole m_Hole;

		float m_GroundFriction = 0.5f;

		std::vector<Obstacle> m_Obstacles;
		std::vector<BoostPad> m_BoostPads;

		friend class LevelManager;
	};

	class LevelManager : public ReferenceCounted
	{
	public:
		LevelManager();
		virtual ~LevelManager();

		void OnUpdate(float time, float deltaTime);
		void SetViewportSize(uint32 width, uint32 height);

		void BindLevel(LevelType type, Ref<Level> level, bool setActive = false);
		void SetActiveLevelType(LevelType type);

		Ref<Level> GetActiveLevel() const;
		Ref<Level> GetLevel(LevelType type) const;
	
		Ref<Renderer> GetRenderer() const { return m_Renderer; }
	private:
		uint32 m_ViewportWidth = 0;
		uint32 m_ViewportHeight = 0;

		Ref<Renderer> m_Renderer;

		LevelType m_ActiveLevelType = LevelType::None;

		std::unordered_map<LevelType, Ref<Level>> m_LevelBindings;
	};

}