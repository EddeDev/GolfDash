#pragma once

#include "Window.h"
#include "Mouse.h"

#include "Renderer.h"

#include "Camera.h"
#include "Ball.h"
#include "Hole.h"

namespace gd {

	class GolfDash
	{
	public:
		GolfDash();

		void Run();

		Ref<Renderer> GetRenderer() const { return m_Renderer; }
		Ref<Mouse> GetMouse() const { return m_Mouse; }
	
		Ref<Ball> GetBall() { return m_Ball; }
		const Ref<Ball> GetBall() const { return m_Ball; }

		Ref<Hole> GetHole() { return m_Hole; }
		const Ref<Hole> GetHole() const { return m_Hole; }

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		static GolfDash& Get() { return *s_Instance; }
	private:
		void RenderBackground();
		void SetViewportSize(uint32 width, uint32 height);
	private:
		static GolfDash* s_Instance;

		Ref<Window> m_Window;
		Ref<Mouse> m_Mouse;
		bool m_Running = true;

		Ref<Renderer> m_Renderer;
		Camera m_Camera;

		Ref<Ball> m_Ball;
		Ref<Hole> m_Hole;

		Ref<Texture> m_GrassTexture;

		uint32 m_ViewportWidth = 0;
		uint32 m_ViewportHeight = 0;
	};

}