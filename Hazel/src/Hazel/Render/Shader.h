#pragma once

namespace Hazel {

	class Shader
	{
	public:
		Shader(std::string& vertexSrc, std::string& fragmentSrc);
		~Shader();

		void Bind()const;
		void UnBind()const;
	private:
		uint32_t m_RendererID;
	};

}

