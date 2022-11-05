#pragma once

#include "Window.h"

#include "Shader.h"

namespace gd {

	class GolfDash
	{
	public:
		GolfDash();

		void Run();
	private:
		void OnInit();
		void OnUpdate();

		void SetViewportSize(uint32 width, uint32 height);
	private:
		Ref<Window> m_Window;
		bool m_Running = true;

		Ref<Shader> m_Shader;

		uint32 m_ViewportWidth = 0;
		uint32 m_ViewportHeight = 0;
	};

}