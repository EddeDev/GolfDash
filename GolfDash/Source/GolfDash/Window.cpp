#include "GolfDashPCH.h"
#include "Window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace gd {

	Window::Window(const WindowConfig& config)
	{
		m_Data.Width = config.Width;
		m_Data.Height = config.Height;
		m_Data.Title = config.Title;

		if (!glfwInit())
		{
			// Assert
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
			// Assert
			return;
		}

		CenterWindow();
		SetCallbacks();
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::CreateContext() const
	{
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glfwMakeContextCurrent(m_Window);
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

		glfwSetFramebufferSizeCallback(m_Window, [](auto window, auto width, auto height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			for (auto& callback : data.FramebufferSizeCallbacks)
				callback(width, height);
		});
	}

}