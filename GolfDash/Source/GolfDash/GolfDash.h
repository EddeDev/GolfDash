#pragma once

#include "Window.h"
#include "Mouse.h"

#include "Level.h"

namespace gd {

	class GolfDash
	{
	public:
		GolfDash();

		void Run();

		Ref<Window> GetWindow() const { return m_Window; }
		Ref<Mouse> GetMouse() const { return m_Mouse; }
		Ref<Level> GetLevel() const { return m_Level; }

		static GolfDash& Get() { return *s_Instance; }
	private:
		static GolfDash* s_Instance;

		Ref<Window> m_Window;
		Ref<Mouse> m_Mouse;
		Ref<Level> m_Level;
		bool m_Running = true;
	};

}