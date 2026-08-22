module;

export module Editor.Application;

import std;

import Helpers.Errors;
import Editor.Window;

namespace Editor
{
	export class Application
	{
	public:
		Application(const std::string& _Name, int _Width, int _Height)
			: m_Window(_Name, _Width, _Height) {}
		virtual ~Application();

		void Run();

	private:
		Window m_Window;

		Result<void> Init();
		void Clean();
	};
}