module;

module Editor.Layers;

import std;

namespace Editor
{
#pragma region Layer
	Layer::~Layer()
	{

	}

	void Layer::OnAttach()
	{

	}

	void Layer::OnDetach()
	{

	}

	void Layer::OnEvent()
	{

	}

	void Layer::OnUpdate(float _Timestamp)
	{

	}

	void Layer::OnRender()
	{

	}

	void Layer::Suspend()
	{

	}

	bool Layer::IsInitialized() const
	{
		return m_Initialized;
	}

	bool Layer::HasPendingTransition() const
	{
		return static_cast<bool>(m_PendingTransition);
	}

	std::unique_ptr<Layer> Layer::TakePendingTransition()
	{
		return std::move(m_PendingTransition);
	}

	void Layer::QueueTransition(std::unique_ptr<Layer> _Layer)
	{
		m_PendingTransition = std::move(_Layer);
	}
#pragma endregion class implementations

#pragma region LayerStack
	LayerStack::~LayerStack()
	{
		Clear();
	}

	void LayerStack::PushLayer(std::unique_ptr<Layer> _Layer)
	{
		_Layer->OnAttach();
		m_Data.push_back(std::move(_Layer));
	}

	void LayerStack::Clear()
	{
		for (std::unique_ptr<Layer>& Entry : m_Data)
			Entry->OnDetach();

		m_Data.clear();
	}

	void LayerStack::OnEvent()
	{
		for (const std::unique_ptr<Layer>& Entry : m_Data)
			Entry->OnEvent();
	}

	void LayerStack::OnUpdate(float _Timestamp)
	{
		for (const std::unique_ptr<Layer>& Entry : m_Data)
			Entry->OnUpdate(_Timestamp);

		// Second pass: perform any transitions requested during the update
		// above. Done outside the loop that calls OnUpdate so we never
		// destroy/replace a layer while it (or the loop) is still running.
		for (std::unique_ptr<Layer>& Entry : m_Data)
		{
			if (!Entry->HasPendingTransition())
				continue;

			std::unique_ptr<Layer> NewLayer = Entry->TakePendingTransition();
			Entry->OnDetach();
			NewLayer->OnAttach();
			Entry = std::move(NewLayer);
		}
	}

	void LayerStack::OnRender()
	{
		for (const std::unique_ptr<Layer>& Entry : m_Data)
		{
			if (Entry->IsInitialized())
				Entry->OnRender();
		}
	}
#pragma endregion class implementations
}
