#include "hzpch.h"
#include "Hazel/Render/Renderer.h"
#include "Hazel/Render/RenderCommand.h"
#include "Hazel/Render/VertexArray.h"

namespace Hazel {
	

	void Renderer::BeginScene() {

	}
	void Renderer::EndScene() {

	}
	void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray) {
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}