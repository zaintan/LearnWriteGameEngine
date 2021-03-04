#pragma once

#include "Hazel/Render/RendererAPI.h"

namespace Hazel {

	class VertexArray;

	class Renderer
	{
	public:

		static void BeginScene();
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray> &vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}

