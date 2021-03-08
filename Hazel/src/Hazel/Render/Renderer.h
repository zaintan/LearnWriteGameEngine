#pragma once
#include <memory>

#include "glm/glm.hpp"

#include "Hazel/Render/RendererAPI.h"

namespace Hazel {

	class VertexArray;
	class Shader;
	class OrthographicCamera;

	class Renderer
	{
	public:

		static void Init();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray> &vertexArray, const std::shared_ptr<Shader> &shader, const::glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		
		static void OnWindowResize(uint32_t width, uint32_t height);

	//private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}

