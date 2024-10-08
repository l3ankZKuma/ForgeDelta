#pragma once

namespace ForgeDelta {
	class Event;
	class TimeStep;
}

namespace ForgeDelta {

	class Layer {
	public:
		constexpr static std::string_view DefaultName = "Layer";
		Layer(const std::string_view name = DefaultName);
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(TimeStep ts) = 0;
		virtual void OnEvent(Event& event) = 0;
		virtual void OnImGuiRender() = 0;

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}
