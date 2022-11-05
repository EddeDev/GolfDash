#pragma once

#include "Window.h"

#include "Shader.h"
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

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
		Ref<Pipeline> m_Pipeline;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		uint32 m_ViewportWidth = 0;
		uint32 m_ViewportHeight = 0;
	};

}