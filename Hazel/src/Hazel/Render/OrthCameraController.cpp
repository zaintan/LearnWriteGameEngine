#include "hzpch.h"
#include "Hazel/Render/OrthCameraController.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel {


	OrthCameraController::OrthCameraController(float aspectRatio, bool rotation)
		:m_Rotation(rotation)
		, m_CameraRotation(0.0f)
		, m_CameraTranslationSpeed(5.0f)
		, m_CameraRotationSpeed(180.0f)
		, m_ZoomLevel(1.0f)
		, m_AspectRatio(aspectRatio)
		, m_CameraPosition(glm::vec3(0.0f,0.0f,0.0f))
		, m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}
	void OrthCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		m_Camera.SetPosition(m_CameraPosition);

		if (m_Rotation) {
			if (Input::IsKeyPressed(HZ_KEY_A))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(HZ_KEY_D))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}	

		m_CameraTranslationSpeed = m_ZoomLevel;
	}
	void OrthCameraController::OnEvent(Event & e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthCameraController::OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthCameraController::OnMouseScrolled));
	}
	bool OrthCameraController::OnMouseScrolled(MouseScrolledEvent & e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthCameraController::OnWindowResized(WindowResizeEvent & e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}
