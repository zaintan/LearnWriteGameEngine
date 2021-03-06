#include "hzpch.h"
#include "Hazel/Render/VertexArray.h"

#include "Hazel/Render/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {

	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			return nullptr;
		case RendererAPI::API::OpenGL:
			//return
			return CreateRef<OpenGLVertexArray>();
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}