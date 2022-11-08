#pragma once

#include "Renderer.h"
#include "Camera.h"

#include "Ball.h"
#include "Hole.h"
#include "Obstacle.h"

namespace gd {

	class Level : public ReferenceCounted
	{
	public:
		Level(uint32 viewportWidth, uint32 viewportHeight);
		virtual ~Level() {}

		void OnUpdate(float time, float deltaTime);
		void SetViewportSize(uint32 width, uint32 height);

		Ref<Renderer> GetRenderer() const { return m_Renderer; }

		Ball& GetBall() { return m_Ball; }
		const Ball& GetBall() const { return m_Ball; }

		Hole& GetHole() { return m_Hole; }
		const Hole& GetHole() const { return m_Hole; }

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }
	private:
		void RenderBackground();
	private:
		uint32 m_ViewportWidth = 0;
		uint32 m_ViewportHeight = 0;

		Ref<Renderer> m_Renderer;
		Ref<Texture> m_GrassTexture;

		// Entities
		Camera m_Camera;
		Ball m_Ball;
		Hole m_Hole;

		std::vector<Obstacle> m_Obstacles;
	};

}