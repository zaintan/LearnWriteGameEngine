#pragma once

#include <Hazel.h>

namespace Hazel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		Hazel::OrthCameraController m_CameraController;

		// Temp
		Ref<FrameBuffer> m_Framebuffer;
		//Ref<Texture2D> m_CheckerboardTexture;

		Ref<Scene> m_ActiveScene;
		Ref<Entity> m_SquareEntity;

		//glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};

}
