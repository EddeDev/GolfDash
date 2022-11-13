#include "GolfDashPCH.h"
#include "Window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stb_image.h>

namespace gd {

	Window::Window(const WindowConfig& config)
	{
		m_Data.Width = config.Width;
		m_Data.Height = config.Height;
		m_Data.Title = config.Title;
		m_Data.VSync = config.VSync;

		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW!" << std::endl;
			return;
		}

		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		if (config.Fullscreen)
		{
			auto monitor = glfwGetPrimaryMonitor();
			auto videomode = glfwGetVideoMode(monitor);
			m_Window = glfwCreateWindow(videomode->width, videomode->height, m_Data.Title.c_str(), monitor, nullptr);
		}
		else
		{
			m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}

		if (!m_Window)
		{
			std::cerr << "Unable to create GLFW window!" << std::endl;
			return;
		}

		// Get window size
		int32 width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		m_Data.Width = width;
		m_Data.Height = height;

		// Get framebuffer size
		int32 framebufferWidth, framebufferHeight;
		glfwGetFramebufferSize(m_Window, &framebufferWidth, &framebufferHeight);
		m_Data.FramebufferWidth = framebufferWidth;
		m_Data.FramebufferHeight = framebufferHeight;

		CenterWindow();
		SetCallbacks();
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::CreateContext() const
	{
		glfwMakeContextCurrent(m_Window);

		int32 status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!status)
		{
			std::cerr << "Could not load OpenGL functions!" << std::endl;
			return;
		}

		glfwSwapInterval(m_Data.VSync ? 1 : 0);
	}

	void Window::ShowWindow() const
	{
		glfwShowWindow(m_Window);
	}

	void Window::PollEvents() const
	{
		glfwPollEvents();
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(m_Window);
	}

	void Window::SetTitle(const std::string& title)
	{
		if (m_Data.Title != title)
		{
			glfwSetWindowTitle(m_Window, title.c_str());
			m_Data.Title = title;
		}
	}

	void Window::SetVSync(bool enabled)
	{
		if (m_Data.VSync != enabled)
		{
			glfwSwapInterval(enabled ? 1 : 0);
			m_Data.VSync = enabled;
		}
	}

	void Window::SetCursor(Ref<Texture> texture) const
	{
		GLFWimage image = {};
		image.width = texture->GetInfo().Width;
		image.height = texture->GetInfo().Height;
		image.pixels = texture->GetInfo().Data;

		GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
		glfwSetCursor(m_Window, cursor);
	}

	void Window::SetCursorVisible(bool visible) const
	{
		if (visible)
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
		else
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	float Window::GetTime() const
	{
		return static_cast<float>(glfwGetTime());
	}

	bool Window::IsMouseButtonDown(uint32 button) const
	{
		int32 state = glfwGetMouseButton(m_Window, button);
		return state == GLFW_PRESS;
	}

	glm::vec2 Window::GetMousePosition() const
	{
		double xpos, ypos;
		glfwGetCursorPos(m_Window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	void Window::CenterWindow() const
	{
		auto videomode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(m_Window, (videomode->width - m_Data.Width) / 2, (videomode->height - m_Data.Height) / 2);
	}

	void Window::SetCallbacks()
	{
		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetWindowCloseCallback(m_Window, [](auto window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			for (auto& callback : data.CloseCallbacks)
				callback();
		});

		glfwSetWindowSizeCallback(m_Window, [](auto window, auto width, auto height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			for (auto& callback : data.SizeCallbacks)
				callback(width, height);
		});

		glfwSetFramebufferSizeCallback(m_Window, [](auto window, auto width, auto height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.FramebufferWidth = width;
			data.FramebufferHeight = height;
			for (auto& callback : data.FramebufferSizeCallbacks)
				callback(width, height);
		});

		glfwSetMouseButtonCallback(m_Window, [](auto window, auto button, auto action, auto mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
			{
				for (auto& callback : data.MouseButtonPressCallbacks)
					callback(button);
				break;
			}
			case GLFW_RELEASE:
			{
				for (auto& callback : data.MouseButtonReleaseCallbacks)
					callback(button);
				break;
			}
			}
		});
	}

}