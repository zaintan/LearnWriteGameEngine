#include "hzpch.h"
#include "Hazel/Render/Renderer.h"
#include "Hazel/Render/RenderCommand.h"
#include "Hazel/Render/VertexArray.h"
#include "Hazel/Render/Shader.h"
#include "Hazel/Render/OrthographicCamera.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {
	
	Renderer::SceneData *Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(const OrthographicCamera& camera) {
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene() {

	}
	void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray, const std::shared_ptr<Shader> &shader, const::glm::mat4& transform) {
		shader->Bind();
		auto s = std::dynamic_pointer_cast<OpenGLShader>(shader);
		s->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		s->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}