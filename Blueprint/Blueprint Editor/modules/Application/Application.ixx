module;

export module Editor.Application;

import std;

import Helpers.Errors;
import Editor.Window;
import Editor.ImGuiLayer;
import Editor.Layers;

namespace Editor
{
	export class Application
	{
	public:
		Application(const std::string& _Name, int _Width, int _Height);
		~Application();

		// Template body must live here (interface unit), not in a .cpp
		// implementation unit — otherwise it isn't reachable for
		// instantiation by files that import this module.
		template<std::derived_from<Layer> T, typename ... Args>
		T& PushLayer(Args&& ... _Args)
		{
			auto NewLayer = std::make_unique<T>(std::forward<Args>(_Args)...);
			T& Ref = *NewLayer;
			m_LayerStack.PushLayer(std::move(NewLayer));
			return Ref;
		}

		void Run();

	private:
		Window m_Window;
		LayerStack m_LayerStack;

		Result<void> Init();
		void Clean();
	};
}
