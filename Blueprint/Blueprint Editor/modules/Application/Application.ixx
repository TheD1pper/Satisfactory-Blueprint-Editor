module;

export module Editor.Application;

import std;

import Helpers.Errors;
import Editor.Window;
import Editor.ImGuiLayer;

namespace Editor
{
	export class Application
	{
	public:
		Application(const std::string& _Name, int _Width, int _Height);
		virtual ~Application();

		void Run();

	private:
		Window m_Window;
		std::vector<std::unique_ptr<>
		ImGuiLayer m_UiLayer;

		Result<void> Init();
		void Clean();
	};
}
