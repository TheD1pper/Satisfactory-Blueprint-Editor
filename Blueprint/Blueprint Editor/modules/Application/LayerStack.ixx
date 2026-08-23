module;

export module Editor.Layers;

import std;

namespace Editor
{
	export class Layer
	{
	public:
		virtual ~Layer();

		virtual void OnAttach();
		virtual void OnDetach();

		virtual void OnEvent();
		virtual void OnUpdate(float _Timestamp);
		virtual void OnRender();

		// Template body must live here (interface unit), not in a .cpp
		// implementation unit — otherwise it isn't reachable for
		// instantiation by files that import this module.
		template<std::derived_from<Layer> T, typename ... Args>
		void TransitionTo(Args&& ... _Args)
		{
			QueueTransition(std::make_unique<T>(std::forward<Args>(_Args)...));
		}

		virtual void Suspend();

		bool IsInitialized() const;

		bool HasPendingTransition() const;
		std::unique_ptr<Layer> TakePendingTransition();

	protected:
		bool m_Initialized = false;

	private:
		void QueueTransition(std::unique_ptr<Layer> _Layer);

		std::unique_ptr<Layer> m_PendingTransition;
	};

	export class LayerStack
	{
	public:
		~LayerStack();

		void PushLayer(std::unique_ptr<Layer> _Layer);
		void Clear();

		void OnEvent();
		void OnUpdate(float _Timestamp);
		void OnRender();

	private:
		std::vector<std::unique_ptr<Layer>> m_Data;
	};
}
