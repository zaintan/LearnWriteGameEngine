#pragma once

#include <memory>
#include "glm/glm.hpp"

namespace Hazel {

	class OrthographicCamera
	{
	public:

		OrthographicCamera(float left, float right, float bottom, float top);
	
		const glm::vec3& GetPosition()const { return m_Position; }
		void SetPosition(const glm::vec3& position) {
			m_Position = position; 
			recaculateViewMatrix();
		}
		
		const float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { 
			m_Rotation = rotation;
			recaculateViewMatrix();
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:

		void recaculateViewMatrix();

		float     m_Rotation;
		glm::vec3 m_Position;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

	};
}

