#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

namespace gd {

	enum class Side
	{
		Left = 0,
		Right,
		Top,
		Bottom
	};

	// Forward-declaration
	struct Obstacle;

	struct ObstacleUpdateFunctionData
	{
		Obstacle* Instance;

		float Time;
		float DeltaTime;
	};

	using ObstacleUpdateFunction = std::function<void(ObstacleUpdateFunctionData&)>;

	struct Obstacle
	{
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Scale = glm::vec2(1.0f);

		glm::vec4 Color = glm::vec4(1.0f);
		Ref<Texture> Texture;

		// Read-only
		glm::vec2 PositionDelta = glm::vec2(0.0f);
		
		ObstacleUpdateFunction UpdateFunction;

		void Update(ObstacleUpdateFunctionData& data)
		{
			if (UpdateFunction)
			{
				glm::vec2 lastPosition = Position;
				UpdateFunction(data);
				PositionDelta = Position - lastPosition;
			}
		}

		bool Intersects(const glm::vec2& position, const glm::vec2& scale) const
		{
			glm::vec2 halfScale = scale * 0.5f;
			if (position.x >= GetLeftSidePosition().x - halfScale.x)
			{
				if (position.x <= GetRightSidePosition().x + halfScale.x)
				{
					if (position.y <= GetTopSidePosition().y + halfScale.y)
					{
						if (position.y >= GetBottomSidePosition().y - halfScale.y)
							return true;
					}
				}
			}
			return false;
		}

		Side GetNearestSide(const glm::vec2& position) const
		{
			float left = glm::distance(position, GetLeftSidePosition());
			float right = glm::distance(position, GetRightSidePosition());
			float top = glm::distance(position, GetTopSidePosition());
			float bottom = glm::distance(position, GetBottomSidePosition());

			float values[4] = { left, right, top, bottom };
			uint32 smallestValueIndex = 0;
			for (uint32 i = 0; i < 4; i++)
			{
				if (values[i] < values[smallestValueIndex])
					smallestValueIndex = i;
			}
			return static_cast<Side>(smallestValueIndex);
		}

		glm::vec2 GetLeftSidePosition() const { return { Position.x - (Scale.x * 0.5f), Position.y }; }
		glm::vec2 GetRightSidePosition() const { return { Position.x + (Scale.x * 0.5f), Position.y }; }
		glm::vec2 GetTopSidePosition() const { return { Position.x, Position.y + (Scale.y * 0.5f) }; }
		glm::vec2 GetBottomSidePosition() const { return { Position.x, Position.y - (Scale.y * 0.5f) }; }
	};

}