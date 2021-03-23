#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Platform/OpenGL/OpenGLShader.h>
#include <Hazel/Render/Render2D.h>

#include <Hazel/Debug/Instrumentor.h>

#define DESIGN_WIDTH 1280.0f
#define DESIGN_HEIGHT 720.0f
//x / DESIGN_WIDTH * (DESIGN_WIDTH / DESIGN_HEIGHT * 2)
//y / DESIGN_HEIGHT * 2.0f;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");

	Hazel::FrameBufferSpecification fbSpec;
	fbSpec.Width = DESIGN_WIDTH;
	fbSpec.Height = DESIGN_HEIGHT;
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();
	// Update
	{
		HZ_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}
	
	Hazel::Render2D::ResetStats();
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		//m_Framebuffer->Bind();
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
	}
	
	{
		HZ_PROFILE_SCOPE("Renderer Draw");
		
		Hazel::Render2D::BeginScene(m_CameraController.GetCamera());

		float aspect = DESIGN_WIDTH / DESIGN_HEIGHT;

		Hazel::Render2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { aspect * 2, 2.0f }, m_CheckerboardTexture,2.0f);

		Hazel::Render2D::DrawRotatedQuad({ 0.0,0.0 }, { 1.0,1.0 }, 45, { 1.0,0.0,0.0,1.0 });
		//Hazel::Render2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		//Hazel::Render2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });

		Hazel::Render2D::EndScene();
	}
	
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

		ImGui::Begin("Settings");

		auto stats = Hazel::Render2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		uint32_t textureID = m_CheckerboardTexture->GetRendererID();
		ImGui::Image((void*)textureID, ImVec2{ DESIGN_WIDTH, DESIGN_HEIGHT });
		ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}