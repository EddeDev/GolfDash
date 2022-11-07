#include "GolfDashPCH.h"
#include "Hole.h"

#include "GolfDash.h"

namespace gd {

	Hole::Hole(const glm::vec2& position)
		: m_Position(position)
	{
		m_Texture = Ref<Texture>::Create("Assets/Textures/Hole.png");
	}

	void Hole::OnUpdate()
	{
		Ref<Renderer> renderer = GolfDash::Get().GetRenderer();
		renderer->RenderQuad({ m_Position, -0.1f }, glm::vec2(0.1f), { 1.0f, 1.0f, 1.0f, 1.0f }, m_Texture);
	}

}