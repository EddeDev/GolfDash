#pragma once

#include "Texture.h"

namespace gd {

	class Level;

	class Ball
	{
	public:
		Ball() = default;
		Ball(Ref<Level> level, const glm::vec2& position);

		void OnUpdate(float time, float deltaTime);
	private:
		Ref<Level> m_Level;
		Ref<Texture> m_Texture;

		bool m_IsDragging = false;
		bool m_IsInHole = false;
		float m_TimeInHole = 0.0f;

		glm::vec2 m_Position = glm::vec2(0.0f);
		glm::vec2 m_Scale = glm::vec2(0.1f);
		glm::vec2 m_InitialScale = glm::vec2(0.1f);
		glm::vec2 m_Direction = glm::vec2(0.0f);
		glm::vec2 m_Velocity = glm::vec2(0.0f);
		glm::vec2 m_InitialVelocity = glm::vec2(0.0f);
		float m_BallMagnitude = 0.0f;
		float m_InitialBallMagnitude = 0.0f;
	};

}