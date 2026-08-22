module;

#include "glad/gl.h"
#include "GLFW/glfw3.h"

module Editor.Application;

import std;

import Helpers.Errors;
import Editor.Window;

namespace Eh = ErrorHandling;

namespace Editor
{
	Application::~Application()
	{

	}

	void Application::Run()
	{
		auto r_Init = Init();
		if (r_Init)
			std::print("{}", r_Init.error().GetLogMessage());

		while (!m_Window.ShouldClose())
		{
			m_Window.OnUpdate();
		}
	}

	Result<void> Application::Init()
	{
		if (!glfwInit())
			return Eh::MakeError(Eh::Init::Glfw, "Could not initialize glfw");
		return {};

	}

	void Application::Clean()
	{

	}
} 