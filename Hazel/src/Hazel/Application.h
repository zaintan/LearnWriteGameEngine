#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Window.h"

#include "Hazel/LayerStack.h"
//#include "Hazel/ImGui/ImGuiLayer.h"


namespace Hazel {
	
	class WindowCloseEvent;
	class ImGuiLayer;


	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void onEvent(Event& event);
		bool onWindowClose(WindowCloseEvent& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }		
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}