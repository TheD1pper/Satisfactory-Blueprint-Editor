module;

export module Editor.ImGuiLayer;

import std;

import Helpers.Errors;
import Editor.Window;
import Editor.Layers;

namespace Editor
{
	export class ImGuiLayer : public Layer
	{
	public:
		~ImGuiLayer();

		Result<void> Init(Window& _Window);
		void Shutdown();

		void OnRender() override;

		void Begin();
		void End();
	};
}
