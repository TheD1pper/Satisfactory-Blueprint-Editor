module;
#include <Windows.h>
#include <stdio.h>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
module Editor.Window;

import std;

import Helpers.Errors;

namespace Eh = ErrorHandling;

namespace Editor
{
	Window::Window(const std::string& _Title, int _Width, int _Height)
		: m_Title(_Title),
		m_Width(_Width),
		m_Height(_Height)
	{
	}

	Window::Window(const WindowSpecification& _Specification)
		: m_Title(_Specification.Title),
		m_Width(_Specification.Width), 
		m_Height(_Specification.Height)
	{
	}

	Window::~Window()
	{
		Shutdown();
	}

	Result<void> Window::Init()
	{
		if (!glfwInit())
		{
			glfwTerminate();
			return MakeError(Eh::Glfw::Initialization, "Could not initialize glfw");
		}
		m_GlfwInitialized = true;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Handle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
		if (!m_Handle)
		{
			Shutdown();
			return MakeError(Eh::Glfw::WindowCreation, "Could not create a window");
		}

		glfwSetWindowUserPointer(m_Handle, this);
		glfwMakeContextCurrent(m_Handle);
		glfwSetFramebufferSizeCallback(m_Handle, FramebufferSizeCallback);

		if (!gladLoadGL(glfwGetProcAddress))
		{
			Shutdown();
			return MakeError(Eh::Glad::Initialization, "Could not initialize glad");
		}

		glViewport(0, 0, m_Width, m_Height);

		return {};
	}

	void Window::Shutdown()
	{
		if (m_Handle)
		{
			glfwDestroyWindow(m_Handle);
			m_Handle = nullptr;
		}

		if (m_GlfwInitialized)
		{
			glfwTerminate();
			m_GlfwInitialized = false;
		}
	}

	void Window::PollEvents()
	{
		if (IsKeyPressed(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(m_Handle, true);

		glfwPollEvents();
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_Handle);
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Handle);
	}

	bool Window::IsKeyPressed(int _Key) const
	{
		return glfwGetKey(m_Handle, _Key) == GLFW_PRESS;
	}

	void Window::OpenDebugConsole()
	{
#ifdef SBE_PLATFORM_WINDOWS
		if (m_ConsoleOpen)
			return;

		if (!AllocConsole())
			return;

		FILE* Stream = nullptr;
		freopen_s(&Stream, "CONOUT$", "w", stdout);
		freopen_s(&Stream, "CONOUT$", "w", stderr);
		freopen_s(&Stream, "CONIN$", "r", stdin);

		std::ios::sync_with_stdio(true);
		SetConsoleTitleA("Satisfactory Blueprint Editor - Debug Console");
		m_ConsoleOpen = true;
#endif
	}

	void Window::CloseDebugConsole()
	{
#ifdef SBE_PLATFORM_WINDOWS
		if (!m_ConsoleOpen)
			return;

		FILE* Stream;
		freopen_s(&Stream, "NUL", "w", stdout);
		freopen_s(&Stream, "NUL", "w", stderr);
		freopen_s(&Stream, "NUL", "r", stdin);

		FreeConsole();
		m_ConsoleOpen = false;
#endif
	}

	void Window::ToggleDebugConsole()
	{
#ifdef SBE_PLATFORM_WINDOWS
		m_ConsoleOpen ? CloseDebugConsole() : OpenDebugConsole();
#endif
	}

	bool Window::IsDebugConsoleOpen() const
	{
#ifdef SBE_PLATFORM_WINDOWS
		return m_ConsoleOpen;
#endif
	}

	int Window::Width() const
	{
		return m_Width;
	}

	int Window::Height() const
	{
		return m_Height;
	}

	glm::vec2 Window::GetWindowSize() const
	{
		return { m_Width, m_Height };
	}

	GLFWwindow* Window::GetHandle() const
	{
		return m_Handle;
	}

	void Window::OnResize(int _Width, int _Height)
	{
		m_Width = _Width;
		m_Height = _Height;
		glViewport(0, 0, _Width, _Height);
	}

	void Window::FramebufferSizeCallback(GLFWwindow* _Window, int _Width, int _Height)
	{
		if (auto* Self = static_cast<Window*>(glfwGetWindowUserPointer(_Window)))
			Self->OnResize(_Width, _Height);
	}
}
