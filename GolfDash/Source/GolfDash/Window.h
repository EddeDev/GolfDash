#pragma once

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

		void AddCloseCallback(const std::function<void()>& callback) { m_Data.CloseCallbacks.push_back(callback); }
		void AddFramebufferSizeCallback(const std::function<void(uint32 width, uint32 height)>& callback) { m_Data.FramebufferSizeCallbacks.push_back(callback); }
	
		uint32 GetWidth() const { return m_Data.Width; }
		uint32 GetHeight() const { return m_Data.Height; }
	private:
		void CenterWindow() const;;
		void SetCallbacks();
	private:
		GLFWwindow* m_Window = nullptr;

		struct WindowData
		{
			uint32 Width;
			uint32 Height;
			std::string Title;

			std::vector<std::function<void()>> CloseCallbacks;
			std::vector<std::function<void(uint32 width, uint32 height)>> FramebufferSizeCallbacks;
		} m_Data;
	};

}