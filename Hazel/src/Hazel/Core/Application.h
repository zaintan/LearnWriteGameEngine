#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Core/Window.h"

#include "Hazel/Core/LayerStack.h"
//#include "Hazel/ImGui/ImGuiLayer.h"


namespace Hazel {
	
	class WindowResizeEvent;
	class WindowCloseEvent;
	class ImGuiLayer;


	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void onEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool onWindowClose(WindowCloseEvent& event);
		bool onWindowResized(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;
		bool  m_Minimized = false;
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}