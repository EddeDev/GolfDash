#pragma once

#include "Texture.h"

struct GLFWwindow;

namespace gd {

	struct WindowConfig
	{
		uint32 Width = 1280;
		uint32 Height = 720;
		std::string Title = "Game";

		bool Fullscreen = false;
		bool VSync = true;
	};

	class Window : public ReferenceCounted
	{
	public:
		Window(const WindowConfig& config);
		virtual ~Window();

		void CreateContext() const;
		void ShowWindow() const;

		void PollEvents() const;
		void SwapBuffers() const;

		// Callbacks
		void AddCloseCallback(const std::function<void()>& callback) { m_Data.CloseCallbacks.push_back(callback); }
		void AddWindowSizeCallback(const std::function<void(uint32 width, uint32 height)>& callback) { m_Data.SizeCallbacks.push_back(callback); }
		void AddFramebufferSizeCallback(const std::function<void(uint32 width, uint32 height)>& callback) { m_Data.FramebufferSizeCallbacks.push_back(callback); }
		void AddMouseButtonPressCallback(const std::function<void(uint32 button)>& callback) { m_Data.MouseButtonPressCallbacks.push_back(callback); }
		void AddMouseButtonReleaseCallback(const std::function<void(uint32 button)>& callback) { m_Data.MouseButtonReleaseCallbacks.push_back(callback); }
		
		void SetTitle(const std::string& title);
		void SetVSync(bool enabled);

		void SetCursor(Ref<Texture> texture) const;
		void SetCursorVisible(bool visible) const;

		float GetTime() const;

		bool IsMouseButtonDown(uint32 button) const;
		glm::vec2 GetMousePosition() const;

		uint32 GetWidth() const { return m_Data.Width; }
		uint32 GetHeight() const { return m_Data.Height; }

		uint32 GetFramebufferWidth() const { return m_Data.FramebufferWidth; }
		uint32 GetFramebufferHeight() const { return m_Data.FramebufferHeight; }
	private:
		void CenterWindow() const;
		void SetCallbacks();
	private:
		GLFWwindow* m_Window = nullptr;

		struct WindowData
		{
			uint32 Width;
			uint32 Height;
			uint32 FramebufferWidth;
			uint32 FramebufferHeight;
			std::string Title;
			bool VSync;

			// Callbacks
			std::vector<std::function<void()>> CloseCallbacks;
			std::vector<std::function<void(uint32 width, uint32 height)>> SizeCallbacks;
			std::vector<std::function<void(uint32 width, uint32 height)>> FramebufferSizeCallbacks;
			std::vector<std::function<void(uint32 button)>> MouseButtonPressCallbacks;
			std::vector<std::function<void(uint32 button)>> MouseButtonReleaseCallbacks;
		} m_Data;
	};

}