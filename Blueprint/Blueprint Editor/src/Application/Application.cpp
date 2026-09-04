module;
#include <Windows.h>
#include "GLFW/glfw3.h"
module Editor.Application;

import std;

import Helpers.Errors;
import Helpers.FsUtils;
import Services.Manager;
import Editor.Window;
import Editor.ImGuiLayer;
import Editor.Layers;

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

			static bool s_PrevF1 = false;
			bool f9 = m_Window.IsKeyPressed(GLFW_KEY_F9);
			if (f9 and !s_PrevF1)
				m_Window.ToggleDebugConsole();
			s_PrevF1 = f9;

			m_LayerStack.OnUpdate(0.0f);
			m_LayerStack.OnRender();
			m_Window.SwapBuffers();
		}
	}

	Result<void> Application::Init()
	{
		CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

		m_Window.OpenDebugConsole();

		if (auto r_Init = m_Window.Init(); !r_Init)
			return r_Init;

		ImGuiLayer& UiLayer = PushLayer<ImGuiLayer>();
		if (auto r_Init = UiLayer.Init(m_Window); !r_Init)
			return r_Init;

		return {};
	}

	void Application::Clean()
	{
		m_LayerStack.Clear();
		m_Window.Shutdown();
		CoUninitialize();
	}
}
