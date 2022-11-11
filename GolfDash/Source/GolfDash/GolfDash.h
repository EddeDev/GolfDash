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
		Ref<LevelManager> GetLevelManager() const { return m_LevelManager; }

		static GolfDash& Get() { return *s_Instance; }
	private:
		Ref<Level> CreateLevel1() const;
		Ref<Level> CreateLevel2() const;
		Ref<Level> CreateLevel3() const;
	private:
		static GolfDash* s_Instance;

		Ref<Window> m_Window;
		Ref<Mouse> m_Mouse;
		Ref<LevelManager> m_LevelManager;
		bool m_Running = true;
	};

}