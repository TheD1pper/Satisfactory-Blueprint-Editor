module;

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

module Editor.ImGuiLayer;

import std;

import Helpers.Errors;
import Editor.Window;

namespace Eh = ErrorHandling;

namespace Editor
{
	ImGuiLayer::~ImGuiLayer()
	{
		Shutdown();
	}

	Result<void> ImGuiLayer::Init(Window& _Window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& Io = ImGui::GetIO();
		Io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		Io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		if (!ImGui_ImplGlfw_InitForOpenGL(_Window.GetHandle(), true))
		{
			ImGui::DestroyContext();
			return MakeError(Eh::ImGui::GlfwImpl, "Could not initialize ImGui GLFW implementation");
		}

		if (!ImGui_ImplOpenGL3_Init())
		{
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
			return MakeError(Eh::ImGui::OpenGL3Impl, "Could not initialize ImGui OpenGL3 implementation");
		}

		m_Initialized = true;
		return {};
	}

	void ImGuiLayer::Shutdown()
	{
		if (!IsInitialized())
			return;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		m_Initialized = false;
	}

	void ImGuiLayer::OnRender()
	{
		Begin();
		if (ImGui::Button("Load blueprint", { 120, 40 }))
		{
			auto o_Path = FileDialog::OpenBlueprintFile();
			if (o_Path)
				std::println("{}", o_Path->string());


		}

		if (m_Window)
		{
			bool ConsoleOpen = m_Window->IsDebugConsoleOpen();
			if (ImGui::Checkbox("Debug console", &ConsoleOpen))
				m_Window->ToggleDebugConsole();
		}
		End();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();
	}

	void ImGuiLayer::End()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
