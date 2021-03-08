#include "Hazel.h"
#include "Hazel/Core/EntryPoint.h"

//#include <Hazel/Events/KeyEvent.h>

#include "imgui/imgui.h"

#include "Hazel/Render/OrthCameraController.h"
#include "Hazel/Render/Shader.h"
#include "Hazel/Render/Buffer.h"
#include "Hazel/Render/VertexArray.h"
#include "Hazel/Render/Renderer.h"
#include "Hazel/Render/RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f/720.0f)
	{
		
		//square
		m_SquareVA.reset(Hazel::VertexArray::Create());
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
		m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		
		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

		auto oglTextureShader = std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_ShaderLibrary.Get("Texture"));
		oglTextureShader->Bind();
		oglTextureShader->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep ts) override
	{

		m_CameraController.OnUpdate(ts);

		Hazel::RenderCommand::SetClearColor(glm::vec4(0, 0, 0, 1));
		Hazel::RenderCommand::Clear();


		//rotation += 1.0f;

		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

		auto oglFlatColorShader = std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_ShaderLibrary.Get("FlatColor"));

		oglFlatColorShader->Bind();
		oglFlatColorShader->UploadUniformFloat4("u_Color", glm::vec4(m_SquareColor,1.0));

		

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(m_SquareVA, oglFlatColorShader, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");
		m_Texture->Bind();
		Hazel::Renderer::Submit(m_SquareVA, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
		Hazel::Renderer::Submit(m_SquareVA, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}
private:



	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Texture2D> m_Texture, m_ChernoLogoTexture;

	Hazel::OrthCameraController m_CameraController;


	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};


class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		//PushOverlay(new Hazel::ImGuiLayer());
	}

	~Sandbox() {

	}


};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox;
}