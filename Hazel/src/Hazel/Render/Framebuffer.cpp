#include "hzpch.h"
#include "Hazel/Render/Framebuffer.h"
#include "Hazel/Render/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Hazel {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}