#include "hzpch.h"

#include "Hazel/Render/Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Hazel/Render/Renderer.h"
//#include "Hazel/Render/RendererAPI.h"

namespace Hazel {

	Shader* Shader::Create(std::string& vertexSrc, std::string& fragmentSrc) 
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::NONE:    
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			return nullptr;
		case RendererAPI::API::OpenGL:  
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
