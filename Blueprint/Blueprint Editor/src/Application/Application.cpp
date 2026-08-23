module;

module Editor.Application;

import std;

import Helpers.Errors;
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

			m_LayerStack.OnUpdate(0.0f);
			m_LayerStack.OnRender();

			m_Window.SwapBuffers();
		}
	}

	Result<void> Application::Init()
	{
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
	}
}
