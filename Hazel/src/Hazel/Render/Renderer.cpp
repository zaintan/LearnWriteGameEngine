#include "hzpch.h"
#include "Hazel/Render/Renderer.h"
#include "Hazel/Render/RenderCommand.h"
#include "Hazel/Render/VertexArray.h"
#include "Hazel/Render/Shader.h"
#include "Hazel/Render/OrthographicCamera.h"
#include "Hazel/Render/Render2D.h"

#include "Platform/OpenGL/OpenGLShader.h"


namespace Hazel {
	
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Render2D::Init();
	}

	void Renderer::ShutDown()
	{
		Render2D::ShutDown();
	}

	void Renderer::BeginScene(const OrthographicCamera& camera) {
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene() {

	}
	void Renderer::Submit(const Ref<VertexArray> &vertexArray, const Ref<Shader> &shader, const::glm::mat4& transform) {
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
}