#pragma once

namespace Hazel {
	enum class RendererAPI
	{
		NONE   = 0,
		OpenGL = 1//,
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RenderedAPI; };

	private:
		static RendererAPI s_RenderedAPI;
	};
}

