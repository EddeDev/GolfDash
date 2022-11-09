#include "GolfDashPCH.h"
#include "Mouse.h"

namespace gd {

	Mouse::Mouse(Ref<Window> window)
		: m_Window(window)
	{
		m_StateBitFields.resize(GD_MOUSE_BUTTON_LAST);

		m_Window->AddMouseButtonPressCallback([this](uint32 button) { OnMouseButtonPressCallback(button); });
		m_Window->AddMouseButtonReleaseCallback([this](uint32 button) { OnMouseButtonReleaseCallback(button); });

		m_MousePosition = m_Window->GetMousePosition();
	}

	void Mouse::OnUpdate()
	{
		glm::vec2 mousePosition = m_Window->GetMousePosition();
		// mousePosition.y = m_Window->GetFramebufferHeight() - mousePosition.y;
		m_MousePositionDelta = m_MousePosition - mousePosition;
		mousePosition = glm::clamp(mousePosition, { 0.0f, 0.0f }, { m_Window->GetFramebufferWidth(), m_Window->GetFramebufferHeight() });
		m_MousePosition = mousePosition;

		// Reset bit fields
		for (size_t i = 0; i < m_StateBitFields.size(); i++)
			m_StateBitFields[i] = 0;
	}

	void Mouse::OnMouseButtonPressCallback(uint32 button)
	{
		m_StateBitFields[button] |= 0b00000001;
		m_StateBitFields[button] &= 0b11111101;
		m_StateBitFields[button] &= 0b11111011;
	}

	void Mouse::OnMouseButtonReleaseCallback(uint32 button)
	{
		m_StateBitFields[button] &= 0b11111110;
		m_StateBitFields[button] |= 0b00000010;
		m_StateBitFields[button] &= 0b11111011;
	}

	bool Mouse::GetMouseButton(uint32 button) const
	{
		return m_Window->IsMouseButtonDown(button);
	}

	bool Mouse::GetMouseButtonDown(uint32 button) const
	{
		return m_StateBitFields[button] & 0b00000001;
	}

	bool Mouse::GetMouseButtonUp(uint32 button) const
	{
		return (m_StateBitFields[button] & 0b00000010) >> 1;
	}

	glm::vec2 Mouse::GetMouseOrthoPosition(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const
	{
		glm::vec4 result = { m_MousePosition, 0.0f, 1.0f };
		result.x = (result.x / (float)m_Window->GetFramebufferWidth()) * 2.0f - 1.0f;
		result.y = (result.y / (float)m_Window->GetFramebufferHeight()) * 2.0f - 1.0f;
		result.y = -result.y;
		result = glm::inverse(viewMatrix * projectionMatrix) * result;
		return result;
	}

}