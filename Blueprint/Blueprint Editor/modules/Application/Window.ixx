module;

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

export module Editor.Window;

import std;

import Helpers.Errors;

namespace Editor
{
	export class Window
	{
	public:
		Window(const std::string& _Title, int _Width, int _Height);
		~Window();

		Result<void> Init();
		void Shutdown();

		void PollEvents();
		void SwapBuffers();
		bool ShouldClose() const;
		bool IsKeyPressed(int _Key) const;

		void OpenDebugConsole();

		int Width() const;
		int Height() const;
		glm::vec2 GetWindowSize() const;
		GLFWwindow* GetHandle() const;

	private:
		std::string m_Title;
		int m_Width = 0;
		int m_Height = 0;
		GLFWwindow* m_Handle = nullptr;
		bool m_GlfwInitialized = false;

		void OnResize(int _Width, int _Height);

		static void FramebufferSizeCallback(GLFWwindow* _Window, int _Width, int _Height);
	};
}
