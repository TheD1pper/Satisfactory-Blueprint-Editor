module;

export module Editor.ImGuiLayer;

import std;

import Helpers.Errors;
import Editor.Window;

namespace Editor
{
	export class ImGuiLayer
	{
	public:
		~ImGuiLayer();

		Result<void> Init(Window& _Window);
		void Shutdown();

		void Begin();
		void End();

	private:
		bool m_Initialized = false;
	};
}
