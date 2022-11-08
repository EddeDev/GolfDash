#pragma once

#include "Texture.h"

namespace gd {

	class Level;

	class Hole
	{
	public:
		Hole() = default;
		Hole(Ref<Level> level, const glm::vec2& position);

		void OnUpdate();

		void SetPosition(const glm::vec2& position) { m_Position = position; }
		glm::vec2 GetPosition() const { return m_Position; }
	private:
		Ref<Level> m_Level;
		Ref<Texture> m_Texture;

		glm::vec2 m_Position = glm::vec2(0.0f);
	};

}