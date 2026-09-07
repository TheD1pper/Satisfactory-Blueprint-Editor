module;

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define DOCKING_ENABLED 0

module Editor.ImGuiLayer;

import std;

import Helpers.FileDialog;
import Helpers.Errors;
import Services.Manager;
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
#if DOCKING_ENABLED
		Io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif


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

		m_Window = &_Window;
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

		Services::BlueprintManager Manager;
		ImVec2 VpSize = ImGui::GetMainViewport()->Size;

		// Object Browser
		{
			ImGuiWindowFlags BrowserFlags =
			  ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoFocusOnAppearing;

			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSizeConstraints(ImVec2(VpSize.x / 6, VpSize.y), ImVec2(VpSize.x / 4, VpSize.y));

			ImGui::Begin("Object Browser", nullptr, BrowserFlags);
			ImGui::End();
		}
		
		// Object Editor
		{
			ImGuiWindowFlags EditorFlags =
				  ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoCollapse
				| ImGuiWindowFlags_NoFocusOnAppearing;

			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSizeConstraints(ImVec2(VpSize.x, VpSize.y * 0.2f), ImVec2(VpSize.x, VpSize.y * 0.4f));

			ImGui::Begin("Object Editor", nullptr, EditorFlags);
			ImGui::End();
		}

		// Debug Window
		{
			ImGui::Begin("Debug Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::Button("Load blueprint", { 120, 40 }))
			{
				auto o_Path = FileDialog::OpenBlueprintFile();
				if (o_Path)
				{
					auto r_Load = Manager.LoadHeader(*o_Path);
					if (!r_Load)
						m_LoadMessage = r_Load.error().GetLogMessage();
					else
						m_LoadMessage = "Loaded: " + o_Path->string();
				}
			}

			if (m_Window)
			{
				bool ConsoleOpen = m_Window->IsDebugConsoleOpen();
				if (ImGui::Checkbox("Show debug console", &ConsoleOpen))
					m_Window->ToggleDebugConsole();
			}

			if (!m_LoadMessage.empty())
			{
				ImGui::TextWrapped("%s", m_LoadMessage.c_str());
			}

			ImGui::End();
		}

		End();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
#if DOCKING_ENABLED
		ImGui::DockSpaceOverViewport();
#endif 
	}

	void ImGuiLayer::End()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
