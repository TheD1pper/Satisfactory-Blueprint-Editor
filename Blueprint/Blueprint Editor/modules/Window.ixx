module;

#include "glad/gl.h"
#include "GLFW/glfw3.h"

export module Editor.Window;

import std;

namespace Editor
{
	export class Window
	{
	public:
		Window(const std::string _Title, int _Width, int _Height);
		~Window();

		void OnUpdate();
		bool ShouldClose() const;
		GLFWwindow* operator*();

	private:
		GLFWwindow* m_Handle = nullptr;

		void OpenDebugConsole();
	};
}