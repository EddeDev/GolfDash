#pragma once

#include "Window.h"
#include "Mouse.h"

#include "Renderer.h"

namespace gd {

	class GolfDash
	{
	public:
		GolfDash();

		void Run();
	private:
		void OnInit();
		void OnUpdate(float time, float deltaTime);

		void RenderBackground();
		void RenderBall(float time, float deltaTime);
		void RenderHole();

		void SetViewportSize(uint32 width, uint32 height);
	private:
		// Core
		Ref<Window> m_Window;
		Ref<Mouse> m_Mouse;
		bool m_Running = true;

		Ref<Renderer> m_Renderer;

		// Textures
		Ref<Texture> m_GrassTexture;
		Ref<Texture> m_BallTexture;
		Ref<Texture> m_HoleTexture;

		// Camera
		float m_AspectRatio = 0.0f;
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

		// Hole
		glm::vec2 m_HolePosition = glm::vec2(0.0f);

		// Ball
		glm::vec2 m_BallPosition = glm::vec2(0.0f);
		glm::vec2 m_BallDirection = glm::vec2(0.0f);
		glm::vec2 m_BallVelocity = glm::vec2(0.0f);
		glm::vec2 m_InitialBallVelocity = glm::vec2(0.0f);
		float m_BallMagnitude = 0.0f;
		float m_InitialBallMagnitude = 0.0f;

		bool m_IsDragging = false;
		bool m_IsBallInHole = false;
		float m_TimeInHole = 0.0f;

		uint32 m_ViewportWidth = 0;
		uint32 m_ViewportHeight = 0;
	};

}