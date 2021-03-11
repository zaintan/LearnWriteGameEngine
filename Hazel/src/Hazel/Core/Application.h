#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Core/Window.h"

#include "Hazel/Core/LayerStack.h"
//#include "Hazel/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);


namespace Hazel {
	
	class WindowResizeEvent;
	class WindowCloseEvent;
	class ImGuiLayer;


	class Application
	{
	public:
		Application();
		virtual ~Application();



		void onEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool onWindowClose(WindowCloseEvent& event);
		bool onWindowResized(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;
		bool  m_Minimized = false;
		static Application* s_Instance;

		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}