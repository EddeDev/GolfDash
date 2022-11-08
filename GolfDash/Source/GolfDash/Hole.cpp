#include "GolfDashPCH.h"
#include "Hole.h"

#include "Level.h"

namespace gd {

	Hole::Hole(Ref<Level> level, const glm::vec2& position)
		: m_Level(level), m_Position(position)
	{
		m_Texture = Ref<Texture>::Create("Assets/Textures/Hole.png");
	}

	void Hole::OnUpdate()
	{
		glm::vec4 tintColor = { 0.0f, 0.0f, 0.0f, 1.0f }; // temp
		m_Level->GetRenderer()->RenderQuad({m_Position, -0.1f}, glm::vec2(0.1f), tintColor, m_Texture);
	}

}