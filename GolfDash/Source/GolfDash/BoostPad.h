#pragma once

#include "Texture.h"

namespace gd {

	struct BoostPad
	{
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Scale = glm::vec2(0.5f * 0.5f);

		float Rotation = 0.0f;

		glm::vec2 GetDirection() const
		{
			
			glm::vec2 result = { glm::sin(glm::radians(Rotation)), glm::cos(glm::radians(Rotation)) };

			// TODO: temp fix
			const float epsilon = 0.0001f;
			if (result.x > -epsilon && std::signbit(result.x))
				result.x = 0.0f;
			if (result.y > -epsilon && std::signbit(result.y))
				result.y = 0.0f;

			return result;
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

		glm::vec2 GetLeftSidePosition() const { return { Position.x - (Scale.x * 0.5f), Position.y }; }
		glm::vec2 GetRightSidePosition() const { return { Position.x + (Scale.x * 0.5f), Position.y }; }
		glm::vec2 GetTopSidePosition() const { return { Position.x, Position.y + (Scale.y * 0.5f) }; }
		glm::vec2 GetBottomSidePosition() const { return { Position.x, Position.y - (Scale.y * 0.5f) }; }
	};

}