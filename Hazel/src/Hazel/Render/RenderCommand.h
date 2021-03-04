#pragma once

#include <memory>
#include "glm/glm.hpp"

#include "Hazel/Render/RendererAPI.h"

namespace Hazel {

	class VertexArray;
	//class RendererAPI;

	class RenderCommand
	{
	public:
		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); };

	private:
		static RendererAPI* s_RendererAPI;
	};
}

