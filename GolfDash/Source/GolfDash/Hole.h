#pragma once

#include "Texture.h"

namespace gd {

	class Hole : public ReferenceCounted
	{
	public:
		Hole() = default;
		Hole(const glm::vec2& position);
		virtual ~Hole() {}

		void OnUpdate();

		void SetPosition(const glm::vec2& position) { m_Position = position; }
		glm::vec2 GetPosition() const { return m_Position; }
	private:
		glm::vec2 m_Position = glm::vec2(0.0f);
	
		Ref<Texture> m_Texture;
	};

}