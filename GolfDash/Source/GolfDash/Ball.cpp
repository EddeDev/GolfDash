#include "GolfDashPCH.h"
#include "Ball.h"

#include "GolfDash.h"

namespace gd {

#define PRINT(message) std::cout << message << std::endl

	Ball::Ball()
	{
		m_Texture = Ref<Texture>::Create("Assets/Textures/Ball.png");
	}

	void Ball::OnUpdate(float time, float deltaTime)
	{
		Ref<Renderer> renderer = GolfDash::Get().GetRenderer();
		Ref<Mouse> mouse = GolfDash::Get().GetMouse();
		Ref<Hole> hole = GolfDash::Get().GetHole();
		Camera& camera = GolfDash::Get().GetCamera();

		glm::vec2 orthoMousePos = mouse->GetMouseOrthoPosition(camera.ViewMatrix, camera.ProjectionMatrix);
		glm::vec2 ballScale = glm::vec2(0.1f);

		if (mouse->GetMouseButton(GD_MOUSE_BUTTON_LEFT))
		{
			if (glm::epsilonEqual(orthoMousePos, m_Position, ballScale * 0.5f) == glm::bvec2(true))
			{
				if (m_Velocity.x == 0.0f && m_Velocity.y == 0.0f)
					m_IsDragging = true;
			}
		}

		const float maxDragDistance = 1.5f;

		if (m_IsDragging)
		{
			glm::vec2 begin = orthoMousePos;
			glm::vec2 end = m_Position;

			float distance = glm::distance(orthoMousePos, m_Position);
			if (distance < maxDragDistance)
			{
				renderer->RenderLine({ begin, -0.1f }, glm::vec4(0.0f), { end, -0.1f }, glm::vec4(1.0f));
			}
		}

		if (mouse->GetMouseButtonUp(GD_MOUSE_BUTTON_LEFT))
		{
			if (m_IsDragging)
			{
				float distance = glm::distance(orthoMousePos, m_Position);
				if (distance <= ballScale.x)
				{
					PRINT("Ball was dragged within it's boundaries (no force applied!)");
				}
				else if (distance >= maxDragDistance)
				{
					PRINT("Reached the max distance!");
				}
				else
				{
					PRINT("Stopped dragging, adding force! distance: " << distance);

					m_InitialVelocity = glm::normalize(m_Position - orthoMousePos);
					m_InitialVelocity *= glm::sqrt(distance);
					PRINT("Initial velocity: " << m_InitialVelocity.x << ", " << m_InitialVelocity.y);
					m_Velocity = m_InitialVelocity;

					m_Direction.x = m_Velocity.x / glm::abs(m_Velocity.x);
					m_Direction.y = m_Velocity.y / glm::abs(m_Velocity.y);

					m_InitialBallMagnitude = glm::length(m_Velocity);
					m_InitialBallMagnitude = glm::min(m_InitialBallMagnitude, 1.0f);
					m_BallMagnitude = m_InitialBallMagnitude;
				}
				m_IsDragging = false;
			}
		}

		m_Position += m_Velocity * deltaTime;

		const float epsilon = 0.0001f;
		if (m_Velocity.x > epsilon || m_Velocity.x < -epsilon || m_Velocity.y > epsilon || m_Velocity.y < -epsilon)
		{
			const float friction = 0.5f;

			if (m_BallMagnitude > 0.0f)
				m_BallMagnitude -= friction * deltaTime;
			if (m_BallMagnitude < 0.0f)
				m_BallMagnitude = 0.0f;

			m_Velocity.x = (m_BallMagnitude / m_InitialBallMagnitude) * glm::abs(m_InitialVelocity.x) * m_Direction.x;
			m_Velocity.y = (m_BallMagnitude / m_InitialBallMagnitude) * glm::abs(m_InitialVelocity.y) * m_Direction.y;
		}
		else
		{
			m_InitialVelocity = { 0.0f, 0.0f };
			m_Velocity = { 0.0f, 0.0f };
		}

		if (m_Position.x >= camera.GetAspectRatio() - (ballScale.x * 0.5f))
		{
			m_Velocity = { -m_Velocity.x, m_Velocity.y };
			m_Direction.x = -1.0f;
		}

		if (m_Position.x <= -camera.GetAspectRatio() + (ballScale.x * 0.5f))
		{
			m_Velocity = { -m_Velocity.x, m_Velocity.y };
			m_Direction.x = 1.0f;
		}

		if (m_Position.y >= 1.0f - (ballScale.x * 0.5f))
		{
			m_Velocity = { m_Velocity.x, -m_Velocity.y };
			m_Direction.y = -1.0f;
		}

		if (m_Position.y <= -1.0f + (ballScale.x * 0.5f))
		{
			m_Velocity = { m_Velocity.x, -m_Velocity.y };
			m_Direction.y = 1.0f;
		}

		if (m_BallMagnitude < 1.0f)
		{
			float size = 0.35f;
			if (glm::epsilonEqual(m_Position, hole->GetPosition(), ballScale * size) == glm::bvec2(true))
			{
				if (!m_IsInHole)
				{
					m_IsInHole = true;
					m_InitialVelocity = { 0.0f, 0.0f };
					m_Velocity = { 0.0f, 0.0f };
				}

				m_TimeInHole += deltaTime;
				if (m_TimeInHole >= 2.0f)
				{
					m_IsInHole = false;
					m_TimeInHole = 0.0f;
					m_Position = { -0.5f, -0.5f };
				}
			}
		}

		if (!m_IsInHole)
			renderer->RenderQuad({ m_Position, -0.1f }, ballScale, { 1.0f, 1.0f, 1.0f, 1.0f }, m_Texture);
	}

}