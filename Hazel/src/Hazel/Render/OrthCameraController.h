#pragma once

#include "Hazel/Render/OrthographicCamera.h"

namespace Hazel {

	class Timestep;
	class Event;
	class MouseScrolledEvent;
	class WindowResizeEvent;

	class OrthCameraController
	{
	public:
		OrthCameraController(float aspectRatio, bool rotation = false);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		bool  m_Rotation;
		float m_CameraRotation;// = 0.0f;
		float m_CameraTranslationSpeed;// = 5.0f;
		float m_CameraRotationSpeed;// = 180.0f;
		float m_ZoomLevel;// = 1.0f;
		float m_AspectRatio;
		glm::vec3 m_CameraPosition;
		OrthographicCamera m_Camera;
	};

}

