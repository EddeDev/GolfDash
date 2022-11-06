#pragma once

#include "Window.h"

#pragma region MouseCodes
#define GD_MOUSE_BUTTON_1      0
#define GD_MOUSE_BUTTON_2      1
#define GD_MOUSE_BUTTON_3      2
#define GD_MOUSE_BUTTON_4      3
#define GD_MOUSE_BUTTON_5      4
#define GD_MOUSE_BUTTON_6      5
#define GD_MOUSE_BUTTON_7      6
#define GD_MOUSE_BUTTON_8      7
#define GD_MOUSE_BUTTON_LAST   GD_MOUSE_BUTTON_8
#define GD_MOUSE_BUTTON_LEFT   GD_MOUSE_BUTTON_1
#define GD_MOUSE_BUTTON_RIGHT  GD_MOUSE_BUTTON_2
#define GD_MOUSE_BUTTON_MIDDLE GD_MOUSE_BUTTON_3
#pragma endregion MouseCodes

namespace gd {

	class Mouse : public ReferenceCounted
	{
	public:
		Mouse(Ref<Window> window);
		virtual ~Mouse() {}

		void OnUpdate();

		bool GetMouseButton(uint32 button) const;
		bool GetMouseButtonDown(uint32 button) const;
		bool GetMouseButtonUp(uint32 button) const;

		glm::vec2 GetMousePosition() const { return m_MousePosition; }
		glm::vec2 GetMouseOrthoPosition(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const;
	private:
		void OnMouseButtonPressCallback(uint32 button);
		void OnMouseButtonReleaseCallback(uint32 button);
	private:
		Ref<Window> m_Window;
		std::vector<uint8> m_StateBitFields;

		glm::vec2 m_MousePosition = glm::vec2(0.0f);
		glm::vec2 m_MousePositionDelta = glm::vec2(0.0f);
	};

}