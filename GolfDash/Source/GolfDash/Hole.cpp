#include "GolfDashPCH.h"
#include "Hole.h"

#include "Level.h"

namespace gd {

	Hole::Hole(Ref<Level> level, const glm::vec2& position)
		: m_Level(level), m_Position(position)
	{
		m_Texture = Texture::Create("Assets/Textures/Hole.psd");
		m_FlagTexture = Texture::Create("Assets/Textures/Hole_Flag.psd");
		m_FlagStickTexture = Texture::Create("Assets/Textures/Hole_Flag_Stick.psd");
	}

	void Hole::OnUpdate(float time)
	{
		m_Level->GetRenderer()->RenderQuad({ m_Position, -0.1f }, glm::vec2(0.1f), glm::vec4(1.0f), m_Texture);

		glm::vec2 stickOffset = { 0.052f, 0.08f };
		m_Level->GetRenderer()->RenderQuad({ m_Position + stickOffset, -0.05f}, glm::vec2(0.2f), glm::vec4(1.0f), m_FlagStickTexture);

		glm::vec2 flagOffset = stickOffset + glm::vec2(-0.049f, 0.039f);
		m_Level->GetRenderer()->RenderQuad({ m_Position + flagOffset, -0.04f }, glm::vec2(0.1f), glm::vec4(1.0f), m_FlagTexture, 1.0f, true);
	}

}