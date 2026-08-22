module;

#include <Windows.h>
#include <stdio.h>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

module Editor.Window;

import std;

namespace Editor
{
	Window::Window(const std::string _Title, int _Width, int _Height)
	{
		m_Handle = glfwCreateWindow(_Width, _Height, _Title.c_str(), 0, 0);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Handle);
		glfwTerminate();
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Handle);
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Handle);
	}

	void Window::OpenDebugConsole()
	{
#ifdef SBE_PLATFORM_WINDOWS
		AllocConsole();

		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		freopen_s(&fp, "CONIN$", "r", stdin);

		std::ios::sync_with_stdio(true);
		SetConsoleTitleA("Blueprint Editor - Debug Console");
#endif
	}

	GLFWwindow* Window::operator*()
	{
		return m_Handle;
	}
}