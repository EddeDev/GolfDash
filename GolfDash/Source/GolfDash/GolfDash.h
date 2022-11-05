#pragma once

#include "Window.h"

namespace gd {

	class GolfDash
	{
	public:
		GolfDash();

		void Run();
	private:
		void OnInit();
		void OnUpdate();
	private:
		Ref<Window> m_Window;

		bool m_Running = true;
	};

}