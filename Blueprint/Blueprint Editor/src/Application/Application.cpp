module;

#include "imgui.h"

module Editor.Application;

import std;

import Helpers.Errors;
import Editor.Window;
import Editor.ImGuiLayer;

namespace Editor
{
	Application::Application(const std::string& _Name, int _Width, int _Height)
		: m_Window(_Name, _Width, _Height) {}

	Application::~Application()
	{
		Clean();
	}

	void Application::Run()
	{
		auto r_Init = Init();
		if (!r_Init)
		{
			std::print("{}", r_Init.error().GetLogMessage());
			return;
		}

		while (!m_Window.ShouldClose())
		{
			m_Window.PollEvents();

			m_UiLayer.Begin();
			ImGui::ShowDemoWindow();
			m_UiLayer.End();

			m_Window.SwapBuffers();
		}
	}

	Result<void> Application::Init()
	{
		m_Window.OpenDebugConsole();

		if (auto r = m_Window.Init(); !r)
			return r;

		if (auto r = m_UiLayer.Init(m_Window); !r)
			return r;

		return {};
	}

	void Application::Clean()
	{
		m_UiLayer.Shutdown();
		m_Window.Shutdown();
	}
}
