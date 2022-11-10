#pragma once

#include "Renderer.h"
#include "Camera.h"

#include "Ball.h"
#include "Hole.h"
#include "Obstacle.h"

namespace gd {

	struct LevelSpecification
	{
		glm::vec2 BallPosition;
		glm::vec2 HolePosition;

		std::vector<Obstacle> Obstacles;

		Ref<Texture> BackgroundTexture;

		// Custom functions
	};

	enum class LevelType : uint32
	{
		None = 0,

		Level1,
		Level2,
		Level3
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

		std::vector<Obstacle>& GetObstacles() { return m_Obstacles; }
		const std::vector<Obstacle>& GetObstacles() const { return m_Obstacles; }
	private:
		void RenderBackground();
		void RenderLogo();
		void RenderObstacles();
	private:
		Ref<Renderer> m_Renderer;
		Ref<Texture> m_BackgroundTexture;
		Ref<Texture> m_LogoTexture;
		Ref<Texture> m_HoleInOneTexture;
		std::vector<Ref<Texture>> m_FeedbackTextures;

		// Entities
		Camera m_Camera;
		Ball m_Ball;
		Hole m_Hole;

		std::vector<Obstacle> m_Obstacles;
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