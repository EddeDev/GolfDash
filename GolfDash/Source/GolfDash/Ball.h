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
		void AddForce(const glm::vec2& force);

		bool IsDragging() const { return m_IsDragging; }
		float GetDragLength() const { return m_DragLength; }
		float GetMaxDragLength() const { return m_MaxDragLength; }
		uint32 GetStrokes() const { return m_Strokes; }

		bool IsInHole() const { return m_IsInHole; }
		bool IsReadyForNextLevel() const { return m_IsReadyForNextLevel; }
		bool HasWon() const { return m_IsReadyForNextLevel; }
		float GetTimeInHole() const { return m_TimeInHole; }
	private:
		Ref<Level> m_Level;
		Ref<Texture> m_Texture;

		glm::vec2 m_Position = glm::vec2(0.0f);
		glm::vec2 m_Scale = glm::vec2(0.1f);
		glm::vec2 m_InitialScale = glm::vec2(0.1f);
		glm::vec2 m_Direction = glm::vec2(0.0f);
		glm::vec2 m_Velocity = glm::vec2(0.0f);
		glm::vec2 m_InitialVelocity = glm::vec2(0.0f);

		float m_DragLength = 0.0f;
		float m_MaxDragLength = 1.0f;
		float m_TimeInHole = 0.0f;
		float m_BallMagnitude = 0.0f;
		float m_InitialBallMagnitude = 0.0f;

		bool m_IsDragging = false;
		bool m_IsInHole = false;
		bool m_IsReadyForNextLevel = false;

		uint32 m_Strokes = 0;
	};

}